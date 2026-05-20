/**
 * @file simcom_ppp.c
 * @brief Implementação do driver do modem celular SIMCom 7663E.
 * @details Este módulo gerencia a comunicação serial de baixo nível com o
 *          modem celular, incluindo inicialização via comandos AT, ativação
 *          da interface de rede PPP via esp_modem, e aquisição de GPS.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#include "simcom_ppp.h"
#include <string.h>
#include <stdlib.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_netif_ppp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

static const char *TAG = "SIMCOM_PPP";

/* --- Definições Internas --- */

/** @brief Tamanho máximo do buffer de resposta de comandos AT. */
#define AT_RESPONSE_BUF_SIZE 512

/** @brief Tempo máximo de espera por resposta de um comando AT (ms). */
#define AT_DEFAULT_TIMEOUT_MS 5000

/** @brief Tempo máximo de espera pelo registro na rede celular (ms). */
#define NETWORK_REG_TIMEOUT_MS 60000

/** @brief Intervalo de polling do watchdog de reconexão (ms). */
#define WATCHDOG_POLL_INTERVAL_MS 10000

/** @brief Bit de evento indicando que a interface PPP obteve IP. */
#define PPP_CONNECTED_BIT BIT0

/** @brief Bit de evento indicando que a interface PPP foi desconectada. */
#define PPP_DISCONNECTED_BIT BIT1

/* --- Variáveis Estáticas --- */

/** @brief Estado operacional atual do modem. */
static simcom_state_t modem_state = SIMCOM_STATE_OFF;

/** @brief Handle do Event Group para sinalização de eventos PPP. */
static EventGroupHandle_t ppp_event_group = NULL;

/** @brief Handle da interface de rede PPP do ESP-NETIF. */
static esp_netif_t *ppp_netif = NULL;

/** @brief Configuração da APN armazenada localmente. */
static simcom_apn_config_t stored_apn = {0};

/** @brief Flag indicando se a conectividade celular está suspensa pelo gerenciador de redes. */
static bool cellular_suspended = false;

/* --- Funções Internas Privadas --- */

/**
 * @brief Envia um comando AT e aguarda a resposta com timeout.
 * @details Transmite a string de comando pela UART, aguarda os bytes de
 *          resposta e verifica se contém a string esperada.
 *
 * @param[in] cmd String do comando AT (ex: "AT\r\n").
 * @param[in] expected_resp Substring esperada na resposta (ex: "OK").
 * @param[out] response_buf Buffer para armazenar a resposta completa (pode ser NULL).
 * @param[in] buf_size Tamanho do buffer de resposta.
 * @param[in] timeout_ms Tempo máximo de espera em milissegundos.
 *
 * @return esp_err_t ESP_OK se a resposta contiver a substring esperada.
 *                   ESP_ERR_TIMEOUT se o timeout expirar sem resposta.
 */
static esp_err_t at_send_cmd(const char *cmd, const char *expected_resp,
                             char *response_buf, size_t buf_size,
                             uint32_t timeout_ms) {
  // Limpa qualquer dado residual no buffer de recepção
  uart_flush_input(SIMCOM_UART_PORT);

  // Envia o comando AT pela UART
  int written =
      uart_write_bytes(SIMCOM_UART_PORT, cmd, strlen(cmd));
  if (written < 0) {
    ESP_LOGE(TAG, "Erro ao escrever comando AT na UART.");
    return ESP_FAIL;
  }

  ESP_LOGD(TAG, "AT TX >> %s", cmd);

  // Buffer local para recepção se o chamador não fornecer
  char local_buf[AT_RESPONSE_BUF_SIZE];
  char *resp = (response_buf != NULL) ? response_buf : local_buf;
  size_t resp_size =
      (response_buf != NULL) ? buf_size : sizeof(local_buf);
  memset(resp, 0, resp_size);

  // Lê a resposta com timeout configurável
  int total_read = 0;
  uint32_t start = xTaskGetTickCount() * portTICK_PERIOD_MS;

  while ((xTaskGetTickCount() * portTICK_PERIOD_MS - start) < timeout_ms) {
    int len = uart_read_bytes(SIMCOM_UART_PORT,
                              (uint8_t *)(resp + total_read),
                              resp_size - total_read - 1,
                              100 / portTICK_PERIOD_MS);
    if (len > 0) {
      total_read += len;
      resp[total_read] = '\0';

      // Verifica se a resposta esperada já chegou
      if (expected_resp != NULL && strstr(resp, expected_resp) != NULL) {
        ESP_LOGD(TAG, "AT RX << %s", resp);
        return ESP_OK;
      }

      // Verifica se recebeu ERROR
      if (strstr(resp, "ERROR") != NULL) {
        ESP_LOGE(TAG, "AT ERROR recebido: %s", resp);
        return ESP_FAIL;
      }
    }
  }

  ESP_LOGW(TAG, "AT Timeout para comando: %s (Resposta parcial: %s)", cmd,
           resp);
  return ESP_ERR_TIMEOUT;
}

/**
 * @brief Callback de eventos IP do ESP-NETIF (recebimento de IP via PPP).
 */
static void on_ip_event(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data) {
  if (event_id == IP_EVENT_PPP_GOT_IP) {
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    ESP_LOGI(TAG,
             "PPP conectado! IP: " IPSTR ", Gateway: " IPSTR
             ", Máscara: " IPSTR,
             IP2STR(&event->ip_info.ip), IP2STR(&event->ip_info.gw),
             IP2STR(&event->ip_info.netmask));

    modem_state = SIMCOM_STATE_PPP_ACTIVE;

    if (ppp_event_group != NULL) {
      xEventGroupSetBits(ppp_event_group, PPP_CONNECTED_BIT);
      xEventGroupClearBits(ppp_event_group, PPP_DISCONNECTED_BIT);
    }
  } else if (event_id == IP_EVENT_PPP_LOST_IP) {
    ESP_LOGW(TAG, "PPP desconectado — IP perdido.");
    modem_state = SIMCOM_STATE_REGISTERED;

    if (ppp_event_group != NULL) {
      xEventGroupSetBits(ppp_event_group, PPP_DISCONNECTED_BIT);
      xEventGroupClearBits(ppp_event_group, PPP_CONNECTED_BIT);
    }
  }
}

/**
 * @brief Executa a sequência completa de inicialização do modem via AT.
 * @details Envia comandos AT em sequência para verificar SIM, desabilitar
 *          eco, registrar na rede e anexar ao serviço de dados GPRS.
 *
 * @return esp_err_t ESP_OK se todos os comandos de inicialização passaram.
 */
static esp_err_t at_init_sequence(void) {
  esp_err_t err;

  // 1. Teste básico de eco — verifica que o modem está vivo
  err = at_send_cmd("AT\r\n", "OK", NULL, 0, 2000);
  if (err != ESP_OK) {
    // Tenta novamente após reset suave
    at_send_cmd("AT+CFUN=1,1\r\n", "OK", NULL, 0, 5000);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    err = at_send_cmd("AT\r\n", "OK", NULL, 0, 3000);
    if (err != ESP_OK) {
      ESP_LOGE(TAG,
               "Modem não responde ao comando AT. Verifique a conexão.");
      return err;
    }
  }

  // 2. Desabilita eco de comandos
  err = at_send_cmd("ATE0\r\n", "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);
  if (err != ESP_OK) {
    return err;
  }

  // 3. Verifica status do SIM Card
  char sim_resp[AT_RESPONSE_BUF_SIZE];
  err = at_send_cmd("AT+CPIN?\r\n", "READY", sim_resp, sizeof(sim_resp),
                    AT_DEFAULT_TIMEOUT_MS);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "SIM Card não detectado ou bloqueado: %s", sim_resp);
    return err;
  }
  ESP_LOGI(TAG, "SIM Card ativo e desbloqueado.");

  // 4. Consulta o número IMEI para identificação do dispositivo
  char imei_resp[AT_RESPONSE_BUF_SIZE];
  err = at_send_cmd("AT+GSN\r\n", "OK", imei_resp, sizeof(imei_resp),
                    AT_DEFAULT_TIMEOUT_MS);
  if (err == ESP_OK) {
    ESP_LOGI(TAG, "IMEI do modem: %s", imei_resp);
  }

  // 5. Configura modo de reporte de registro na rede
  at_send_cmd("AT+CREG=1\r\n", "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);
  at_send_cmd("AT+CGREG=1\r\n", "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);

  modem_state = SIMCOM_STATE_AT_READY;
  ESP_LOGI(TAG, "Sequência de inicialização AT concluída.");
  return ESP_OK;
}

/**
 * @brief Aguarda o registro do modem na rede celular com polling.
 *
 * @param[in] timeout_ms Tempo máximo de espera pelo registro.
 *
 * @return esp_err_t ESP_OK se registrado, ESP_ERR_TIMEOUT se expirou.
 */
static esp_err_t wait_for_network_registration(uint32_t timeout_ms) {
  uint32_t start = xTaskGetTickCount() * portTICK_PERIOD_MS;
  char resp[AT_RESPONSE_BUF_SIZE];

  while ((xTaskGetTickCount() * portTICK_PERIOD_MS - start) < timeout_ms) {
    esp_err_t err = at_send_cmd("AT+CREG?\r\n", "+CREG:", resp,
                                sizeof(resp), AT_DEFAULT_TIMEOUT_MS);
    if (err == ESP_OK) {
      // +CREG: 0,1 (registrado na rede local) ou +CREG: 0,5 (roaming)
      if (strstr(resp, ",1") != NULL || strstr(resp, ",5") != NULL) {
        ESP_LOGI(TAG, "Modem registrado na rede celular.");
        return ESP_OK;
      }
    }

    ESP_LOGD(TAG, "Aguardando registro na rede... (%s)", resp);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  ESP_LOGE(TAG, "Timeout ao aguardar registro na rede celular.");
  return ESP_ERR_TIMEOUT;
}

/**
 * @brief Task de watchdog que monitora e reconecta o modem PPP.
 * @details Verifica periodicamente o estado da conexão PPP e, em caso
 *          de queda, tenta reconectar automaticamente.
 *
 * @param[in] pvParameters Parâmetros padrão do FreeRTOS (não utilizado).
 */
static void simcom_watchdog_task(void *pvParameters) {
  ESP_LOGI(TAG, "Watchdog de reconexão ativo. Intervalo: %d ms",
           WATCHDOG_POLL_INTERVAL_MS);

  while (1) {
    vTaskDelay(WATCHDOG_POLL_INTERVAL_MS / portTICK_PERIOD_MS);

    if (cellular_suspended) {
      ESP_LOGD(TAG, "Watchdog celular: conectividade suspensa (Wi-Fi ativo).");
      continue;
    }

    if (modem_state == SIMCOM_STATE_PPP_ACTIVE) {
      // Conexão ativa — nada a fazer
      continue;
    }

    if (modem_state == SIMCOM_STATE_ERROR ||
        modem_state == SIMCOM_STATE_OFF) {
      ESP_LOGW(TAG, "Watchdog: Modem em estado de erro. Tentando reiniciar...");
      if (simcom_ppp_init() != ESP_OK) {
        continue;
      }
    }

    if (modem_state == SIMCOM_STATE_AT_READY) {
      ESP_LOGW(TAG, "Watchdog: Reconfigurando APN...");
      if (simcom_ppp_configure_apn(&stored_apn) != ESP_OK) {
        continue;
      }
    }

    if (modem_state == SIMCOM_STATE_REGISTERED) {
      ESP_LOGW(TAG, "Watchdog: Restabelecendo sessão PPP...");
      simcom_ppp_connect();
    }
  }
}

/* --- Implementação de Funções Públicas --- */

esp_err_t simcom_ppp_init(void) {
  ESP_LOGI(TAG, "Inicializando driver UART para SIMCom 7663E...");

  // Configura a porta serial UART2
  uart_config_t uart_config = {
      .baud_rate = SIMCOM_UART_BAUD_RATE,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_DEFAULT,
  };

  esp_err_t err = uart_param_config(SIMCOM_UART_PORT, &uart_config);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha na configuração UART2: %d", err);
    return err;
  }

  err = uart_set_pin(SIMCOM_UART_PORT, SIMCOM_UART_TX_PIN,
                     SIMCOM_UART_RX_PIN, UART_PIN_NO_CHANGE,
                     UART_PIN_NO_CHANGE);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha na atribuição de pinos UART2: %d", err);
    return err;
  }

  err = uart_driver_install(SIMCOM_UART_PORT, SIMCOM_UART_BUF_SIZE * 2,
                            SIMCOM_UART_BUF_SIZE, 0, NULL, 0);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha na instalação do driver UART2: %d", err);
    return err;
  }

  ESP_LOGI(TAG,
           "UART2 ativa. TX:%d RX:%d @ %d bps. Aguardando boot do modem...",
           SIMCOM_UART_TX_PIN, SIMCOM_UART_RX_PIN, SIMCOM_UART_BAUD_RATE);

  // Aguarda o boot do modem (tempo típico de inicialização: 2-5 segundos)
  vTaskDelay(3000 / portTICK_PERIOD_MS);

  // Executa a sequência de inicialização AT
  err = at_init_sequence();
  if (err != ESP_OK) {
    modem_state = SIMCOM_STATE_ERROR;
    return err;
  }

  // Cria o Event Group para eventos PPP se ainda não existir
  if (ppp_event_group == NULL) {
    ppp_event_group = xEventGroupCreate();
  }

  return ESP_OK;
}

esp_err_t simcom_ppp_configure_apn(const simcom_apn_config_t *apn_config) {
  if (apn_config == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  if (modem_state < SIMCOM_STATE_AT_READY) {
    ESP_LOGE(TAG, "Modem não está pronto para configuração de APN.");
    return ESP_ERR_INVALID_STATE;
  }

  // Armazena a configuração de APN localmente para reconexões
  memcpy(&stored_apn, apn_config, sizeof(simcom_apn_config_t));

  // 1. Aguarda o registro na rede celular
  esp_err_t err = wait_for_network_registration(NETWORK_REG_TIMEOUT_MS);
  if (err != ESP_OK) {
    return err;
  }

  // 2. Anexa ao serviço de dados GPRS
  err = at_send_cmd("AT+CGATT=1\r\n", "OK", NULL, 0, 10000);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha ao anexar ao serviço GPRS.");
    return err;
  }

  // 3. Configura o perfil PDP Context com a APN
  char apn_cmd[128];
  snprintf(apn_cmd, sizeof(apn_cmd),
           "AT+CGDCONT=1,\"IP\",\"%s\"\r\n", apn_config->apn);
  err = at_send_cmd(apn_cmd, "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha ao configurar APN: %s", apn_config->apn);
    return err;
  }

  // 4. Configura autenticação PAP/CHAP se credenciais fornecidas
  if (strlen(apn_config->user) > 0) {
    char auth_cmd[256];
    snprintf(auth_cmd, sizeof(auth_cmd),
             "AT+CGAUTH=1,1,\"%s\",\"%s\"\r\n", apn_config->user,
             apn_config->password);
    at_send_cmd(auth_cmd, "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);
  }

  modem_state = SIMCOM_STATE_REGISTERED;
  ESP_LOGI(TAG, "APN '%s' configurada e modem registrado na rede.",
           apn_config->apn);
  return ESP_OK;
}

esp_err_t simcom_ppp_connect(void) {
  if (modem_state < SIMCOM_STATE_REGISTERED) {
    ESP_LOGE(TAG,
             "Modem não registrado. Configure a APN antes de conectar.");
    return ESP_ERR_INVALID_STATE;
  }

  ESP_LOGI(TAG, "Iniciando sessão PPP via ATD*99#...");

  // 1. Inicializa o ESP-NETIF para PPP se ainda não existir
  if (ppp_netif == NULL) {
    esp_netif_config_t netif_ppp_config = ESP_NETIF_DEFAULT_PPP();
    ppp_netif = esp_netif_new(&netif_ppp_config);
    if (ppp_netif == NULL) {
      ESP_LOGE(TAG, "Falha ao criar a interface de rede PPP.");
      return ESP_FAIL;
    }

    // Registra handler de eventos IP
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID,
                                               on_ip_event, NULL));
  }

  // 2. Comuta o modem para modo de dados PPP
  esp_err_t err =
      at_send_cmd("ATD*99#\r\n", "CONNECT", NULL, 0, 30000);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha ao entrar no modo de dados PPP.");
    modem_state = SIMCOM_STATE_ERROR;
    return err;
  }

  ESP_LOGI(TAG, "Modem em modo de dados PPP. Negociando IP via LCP/IPCP...");

  // 3. Aguarda a alocação de IP pela rede (sinalizado via on_ip_event)
  EventBits_t bits = xEventGroupWaitBits(
      ppp_event_group, PPP_CONNECTED_BIT, pdFALSE, pdTRUE,
      30000 / portTICK_PERIOD_MS);

  if (bits & PPP_CONNECTED_BIT) {
    ESP_LOGI(TAG, "Sessão PPP ativa e operacional.");
    return ESP_OK;
  }

  ESP_LOGE(TAG, "Timeout na negociação PPP — IP não alocado.");
  modem_state = SIMCOM_STATE_ERROR;
  return ESP_ERR_TIMEOUT;
}

esp_err_t simcom_ppp_disconnect(void) {
  if (modem_state != SIMCOM_STATE_PPP_ACTIVE) {
    return ESP_OK; // Já desconectado
  }

  ESP_LOGI(TAG, "Encerrando sessão PPP...");

  // Envia a sequência de escape "+++" para sair do modo de dados
  vTaskDelay(1100 / portTICK_PERIOD_MS); // Guard time obrigatório de 1s
  uart_write_bytes(SIMCOM_UART_PORT, "+++", 3);
  vTaskDelay(1100 / portTICK_PERIOD_MS);

  // Confirma retorno ao modo de comando
  esp_err_t err = at_send_cmd("AT\r\n", "OK", NULL, 0, 3000);
  if (err != ESP_OK) {
    ESP_LOGW(TAG, "Modem não respondeu após escape +++. Forçando hangup.");
    at_send_cmd("ATH\r\n", "OK", NULL, 0, 3000);
  }

  modem_state = SIMCOM_STATE_REGISTERED;
  ESP_LOGI(TAG, "Sessão PPP encerrada. Modem em modo de comandos.");
  return ESP_OK;
}

simcom_state_t simcom_ppp_get_state(void) { return modem_state; }

bool simcom_ppp_is_connected(void) {
  return modem_state == SIMCOM_STATE_PPP_ACTIVE;
}

esp_err_t simcom_ppp_get_gps(simcom_gps_data_t *gps_out) {
  if (gps_out == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  memset(gps_out, 0, sizeof(simcom_gps_data_t));

  // Se em modo PPP, primeiro deve sair do modo de dados
  bool was_ppp = (modem_state == SIMCOM_STATE_PPP_ACTIVE);
  if (was_ppp) {
    simcom_ppp_disconnect();
  }

  // Habilita GPS se ainda não estiver ativo
  at_send_cmd("AT+CGPS=1\r\n", "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);
  vTaskDelay(2000 / portTICK_PERIOD_MS);

  // Solicita dados de posição
  char gps_resp[AT_RESPONSE_BUF_SIZE];
  esp_err_t err = at_send_cmd("AT+CGPSINFO\r\n", "+CGPSINFO:", gps_resp,
                              sizeof(gps_resp), AT_DEFAULT_TIMEOUT_MS);
  if (err != ESP_OK) {
    ESP_LOGW(TAG, "Falha ao obter dados do GPS.");
    if (was_ppp) {
      simcom_ppp_connect();
    }
    return err;
  }

  // Parse da resposta: +CGPSINFO: lat,N/S,lon,E/W,date,time,alt,speed,course
  char *data = strstr(gps_resp, "+CGPSINFO:");
  if (data != NULL) {
    data += strlen("+CGPSINFO:");
    // Verifica se há dados válidos (campo não vazio)
    while (*data == ' ') data++;

    if (*data == ',' || *data == '\r' || *data == '\n') {
      ESP_LOGW(TAG, "GPS sem fix válido.");
      gps_out->valid = false;
      if (was_ppp) {
        simcom_ppp_connect();
      }
      return ESP_ERR_NOT_FOUND;
    }

    // Faz parse dos campos separados por vírgula
    double lat_raw = 0, lon_raw = 0;
    char ns = 'N', ew = 'E';
    float alt = 0, spd = 0;

    int parsed = sscanf(data, "%lf,%c,%lf,%c,%*[^,],%*[^,],%f,%f",
                        &lat_raw, &ns, &lon_raw, &ew, &alt, &spd);

    if (parsed >= 4) {
      // Converte de formato NMEA (ddmm.mmmm) para decimal
      int lat_deg = (int)(lat_raw / 100);
      double lat_min = lat_raw - (lat_deg * 100);
      gps_out->latitude = lat_deg + (lat_min / 60.0);
      if (ns == 'S') gps_out->latitude = -gps_out->latitude;

      int lon_deg = (int)(lon_raw / 100);
      double lon_min = lon_raw - (lon_deg * 100);
      gps_out->longitude = lon_deg + (lon_min / 60.0);
      if (ew == 'W') gps_out->longitude = -gps_out->longitude;

      gps_out->altitude = alt;
      gps_out->speed_kmh = spd;
      gps_out->valid = true;

      ESP_LOGI(TAG, "GPS Fix: Lat=%.6f, Lon=%.6f, Alt=%.1f m, Vel=%.1f km/h",
               gps_out->latitude, gps_out->longitude, gps_out->altitude,
               gps_out->speed_kmh);
    }
  }

  // Reconecta PPP se estava ativo antes da consulta GPS
  if (was_ppp) {
    simcom_ppp_connect();
  }

  return gps_out->valid ? ESP_OK : ESP_ERR_NOT_FOUND;
}

BaseType_t simcom_ppp_watchdog_start(UBaseType_t priority) {
  return xTaskCreate(simcom_watchdog_task, "simcom_watchdog", 4096, NULL,
                     priority, NULL);
}

void simcom_ppp_set_suspended(bool suspend) {
  cellular_suspended = suspend;
  if (suspend) {
    ESP_LOGI(TAG, "Conectividade celular suspensa pelo gerenciador de redundância. Desconectando PPP...");
    simcom_ppp_disconnect();
  } else {
    ESP_LOGI(TAG, "Conectividade celular reativada pelo gerenciador de redundância.");
  }
}

bool simcom_ppp_is_suspended(void) {
  return cellular_suspended;
}
