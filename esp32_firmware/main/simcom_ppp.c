/**
 * @file simcom_ppp.c
 * @brief Implementacao do driver do modem celular SIMCom 7663E.
 * @details Este modulo gerencia a comunicacao serial de baixo nivel com o
 *          modem celular, incluindo inicializacao via comandos AT, ativacao
 *          da interface de rede PPP via esp_modem, e aquisicao de GPS.
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
#include "ble_mobile.h"
#include "stm32_cmd.h"
#include "cJSON.h"

static const char *TAG = "SIMCOM_PPP";

/* --- Definicoes Internas --- */

/** @brief Tamanho maximo do buffer de resposta de comandos AT. */
#define AT_RESPONSE_BUF_SIZE 512

/** @brief Tempo maximo de espera por resposta de um comando AT (ms). */
#define AT_DEFAULT_TIMEOUT_MS 5000

/** @brief Tempo maximo de espera pelo registro na rede celular (ms). */
#define NETWORK_REG_TIMEOUT_MS 60000

/** @brief Intervalo de polling do watchdog de reconexao (ms). */
#define WATCHDOG_POLL_INTERVAL_MS 10000

/** @brief Bit de evento indicando que a interface PPP obteve IP. */
#define PPP_CONNECTED_BIT BIT0

/** @brief Bit de evento indicando que a interface PPP foi desconectada. */
#define PPP_DISCONNECTED_BIT BIT1

/* --- Variaveis Estaticas --- */

/** @brief Estado operacional atual do modem. */
static simcom_state_t modem_state = SIMCOM_STATE_OFF;

/** @brief Handle do Event Group para sinalizacao de eventos PPP. */
static EventGroupHandle_t ppp_event_group = NULL;

/** @brief Handle da interface de rede PPP do ESP-NETIF. */
static esp_netif_t *ppp_netif = NULL;

/** @brief Configuracao da APN armazenada localmente. */
static simcom_apn_config_t stored_apn = {0};

/** @brief Flag indicando se a conectividade celular esta suspensa pelo gerenciador de redes. */
static bool cellular_suspended = false;

/** @brief IMEI lido do modem celular. */
static char modem_imei[32] = "bastao-esp-default";

/* --- Cache de metricas celulares --- */
static int cached_rssi = 99;
static int cached_ber = -1;
static cellular_tech_t cached_tech = CELLULAR_TECH_NONE;
static int cached_mcc = 0;
static int cached_mnc = 0;
static char cached_operator_name[32] = {0};
static int cached_rsrp = -140;
static int cached_rsrq = -20;
static int cached_sinr = -20;
static char cached_ceer[64] = "Nenhum";

/* --- Driver Customizado e Ponte de Dados PPP UART --- */

typedef struct {
    esp_netif_driver_base_t base;
} ppp_driver_t;

static ppp_driver_t ppp_driver;

static esp_err_t ppp_transmit(void *h, void *buffer, size_t len) {
  int written = uart_write_bytes(SIMCOM_UART_PORT, buffer, len);
  if (written < 0) {
    ESP_LOGE(TAG, "Erro ao transmitir dados PPP pela UART.");
    return ESP_FAIL;
  }
  return ESP_OK;
}

static void ppp_free_rx_buffer(void *h, void *buffer) {
  free(buffer);
}

static void ppp_rx_task(void *pvParameters) {
  uint8_t *buffer = malloc(SIMCOM_UART_BUF_SIZE);
  if (buffer == NULL) {
    ESP_LOGE(TAG, "Falha ao alocar buffer para a task RX PPP.");
    vTaskDelete(NULL);
    return;
  }

  ESP_LOGI(TAG, "Tarefa de recepcao PPP iniciada.");

  while (1) {
    if (modem_state == SIMCOM_STATE_PPP_ACTIVE) {
      int len = uart_read_bytes(SIMCOM_UART_PORT, buffer, SIMCOM_UART_BUF_SIZE, pdMS_TO_TICKS(10));
      if (len > 0) {
        // Aloca buffer para transferir a posse ao esp-netif
        uint8_t *net_buf = malloc(len);
        if (net_buf != NULL) {
          memcpy(net_buf, buffer, len);
          esp_err_t err = esp_netif_receive(ppp_netif, net_buf, len, net_buf);
          if (err != ESP_OK) {
            ESP_LOGE(TAG, "Erro ao enviar dados para a pilha PPP: %d", err);
            free(net_buf);
          }
        } else {
          ESP_LOGE(TAG, "Falha ao alocar memoria para dados recebidos do PPP.");
        }
      }
    } else {
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
  free(buffer);
}

/* --- Funcoes Internas Privadas --- */

/**
 * @brief Envia um comando AT e aguarda a resposta com timeout.
 * @details Transmite a string de comando pela UART, aguarda os bytes de
 *          resposta e verifica se contem a string esperada.
 *
 * @param[in] cmd String do comando AT (ex: "AT\r\n").
 * @param[in] expected_resp Substring esperada na resposta (ex: "OK").
 * @param[out] response_buf Buffer para armazenar a resposta completa (pode ser NULL).
 * @param[in] buf_size Tamanho do buffer de resposta.
 * @param[in] timeout_ms Tempo maximo de espera em milissegundos.
 *
 * @return esp_err_t ESP_OK se a resposta contiver a substring esperada.
 *                   ESP_ERR_TIMEOUT se o timeout expirar sem resposta.
 */
static esp_err_t at_send_cmd(const char *cmd, const char *expected_resp,
                             char *response_buf, size_t buf_size,
                             uint32_t timeout_ms) {
  // Limpa qualquer dado residual no buffer de recepcao
  uart_flush_input(SIMCOM_UART_PORT);

  // Envia o comando AT pela UART
  int written =
      uart_write_bytes(SIMCOM_UART_PORT, cmd, strlen(cmd));
  if (written < 0) {
    ESP_LOGE(TAG, "Erro ao escrever comando AT na UART.");
    return ESP_FAIL;
  }

  ESP_LOGD(TAG, "AT TX >> %s", cmd);

  // Buffer local para recepcao se o chamador nao fornecer
  char local_buf[AT_RESPONSE_BUF_SIZE];
  char *resp = (response_buf != NULL) ? response_buf : local_buf;
  size_t resp_size =
      (response_buf != NULL) ? buf_size : sizeof(local_buf);
  memset(resp, 0, resp_size);

  // Le a resposta com timeout configuravel
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

      // Verifica se a resposta esperada ja chegou
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
             ", Mascara: " IPSTR,
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

static void extract_digits(const char *src, char *dst, size_t max_len) {
  size_t d_idx = 0;
  for (size_t s_idx = 0; src[s_idx] != '\0' && d_idx < max_len - 1; s_idx++) {
    if (src[s_idx] >= '0' && src[s_idx] <= '9') {
      dst[d_idx++] = src[s_idx];
    }
  }
  dst[d_idx] = '\0';
}

static esp_err_t simcom_ppp_get_ccid(char *ccid_out, size_t max_len) {
  if (ccid_out == NULL || max_len == 0) {
    return ESP_ERR_INVALID_ARG;
  }
  ccid_out[0] = '\0';

  char resp[AT_RESPONSE_BUF_SIZE];
  esp_err_t err = at_send_cmd("AT+CCID\r\n", "OK", resp, sizeof(resp), AT_DEFAULT_TIMEOUT_MS);
  if (err != ESP_OK) {
    return err;
  }

  char *p = strstr(resp, "+CCID:");
  if (p != NULL) {
    p += 6;
  } else {
    p = resp;
  }

  extract_digits(p, ccid_out, max_len);
  return ESP_OK;
}

static void string_to_upper(char *str) {
  for (int i = 0; str[i]; i++) {
    if (str[i] >= 'a' && str[i] <= 'z') {
      str[i] = str[i] - 'a' + 'A';
    }
  }
}

static void format_status_sms(char *buf, size_t buf_size) {
  snprintf(buf, buf_size,
           "Status Bastao: Bat=%.2fV, PPP=%s, MQTT=%s, Tags=%lu, SIM Slot=%d, CCID=%s, GPS=%s (Lat=%.6f, Lon=%.6f)",
           bastao_current_status.battery_voltage,
           bastao_current_status.ppp_connected ? "Conectado" : "Desconectado",
           bastao_current_status.mqtt_connected ? "Conectado" : "Desconectado",
           (unsigned long)bastao_current_status.tags_read_count,
           bastao_current_status.active_sim_slot,
           bastao_current_status.sim_ccid,
           bastao_current_status.gps_fix ? "OK" : "Sem Fix",
           bastao_current_status.gps_latitude,
           bastao_current_status.gps_longitude);
}

static esp_err_t query_cpsi_metrics(void) {
  char resp[AT_RESPONSE_BUF_SIZE];
  esp_err_t err = at_send_cmd("AT+CPSI?\r\n", "+CPSI:", resp, sizeof(resp), AT_DEFAULT_TIMEOUT_MS);
  if (err != ESP_OK) {
    return err;
  }

  char *p = strstr(resp, "+CPSI:");
  if (p == NULL) {
    return ESP_FAIL;
  }

  p += 6; // Pula "+CPSI:"
  while (*p == ' ') p++;

  if (strncmp(p, "LTE", 3) == 0) {
    char *fields[20] = {0};
    fields[0] = p;
    int f_idx = 1;
    char *c = p;
    
    while (*c != '\0' && f_idx < 20) {
      if (*c == ',') {
        *c = '\0';
        fields[f_idx++] = c + 1;
      }
      c++;
    }
    
    if (f_idx >= 15) {
      cached_rsrq = atoi(fields[11]);
      cached_rsrp = atoi(fields[12]);
      int rssi_cpsi = atoi(fields[13]);
      if (rssi_cpsi != 0) {
        cached_rssi = rssi_cpsi;
      }
      cached_sinr = atoi(fields[14]);
      
      ESP_LOGI(TAG, "CPSI LTE parsed: RSRQ=%d, RSRP=%d, RSSI=%d, SINR=%d",
               cached_rsrq, cached_rsrp, cached_rssi, cached_sinr);
      return ESP_OK;
    }
  }
  return ESP_FAIL;
}

static esp_err_t query_ceer_log(void) {
  char resp[AT_RESPONSE_BUF_SIZE];
  esp_err_t err = at_send_cmd("AT+CEER\r\n", "+CEER:", resp, sizeof(resp), AT_DEFAULT_TIMEOUT_MS);
  if (err != ESP_OK) {
    return err;
  }

  char *p = strstr(resp, "+CEER:");
  if (p == NULL) {
    return ESP_FAIL;
  }

  p += 6; // Pula "+CEER:"
  while (*p == ' ' || *p == '"') p++;

  int idx = 0;
  while (p[idx] != '\0' && p[idx] != '\r' && p[idx] != '\n' && p[idx] != '"' && idx < sizeof(cached_ceer) - 1) {
    cached_ceer[idx] = p[idx];
    idx++;
  }
  cached_ceer[idx] = '\0';

  while (idx > 0 && cached_ceer[idx - 1] == ' ') {
    cached_ceer[idx - 1] = '\0';
    idx--;
  }

  ESP_LOGI(TAG, "CEER Error Log: %s", cached_ceer);
  return ESP_OK;
}

static void process_sms_command(int index, const char *sender, const char *body) {
  ESP_LOGI(TAG, "Processando SMS no index %d de %s: '%s'", index, sender, body);
  
  char resp_msg[1200] = {0};
  bool reply_needed = false;
  
  if (body[0] == '{') {
    cJSON *json = cJSON_Parse(body);
    if (json != NULL) {
      cJSON *cmd_item = cJSON_GetObjectItem(json, "cmd");
      if (cmd_item && cJSON_IsString(cmd_item)) {
        const char *cmd = cmd_item->valuestring;
        char cmd_upper[32] = {0};
        strncpy(cmd_upper, cmd, sizeof(cmd_upper) - 1);
        string_to_upper(cmd_upper);
        
        if (strcmp(cmd_upper, "BUZZER") == 0) {
          stm32_cmd_send_buzzer(STM32_CMD_BUZZER_DOUBLE);
          snprintf(resp_msg, sizeof(resp_msg), "Comando BUZZER executado.");
          reply_needed = true;
        } else if (strcmp(cmd_upper, "RFID") == 0) {
          cJSON *state = cJSON_GetObjectItem(json, "state");
          if (state) {
            bool en = cJSON_IsTrue(state) || state->valueint == 1;
            stm32_cmd_send_rfid_power(en ? 1 : 0);
            snprintf(resp_msg, sizeof(resp_msg), "RFID configurado para %s.", en ? "ON" : "OFF");
          } else {
            stm32_cmd_send_rfid_power(1);
            snprintf(resp_msg, sizeof(resp_msg), "RFID ativado.");
          }
          reply_needed = true;
        } else if (strcmp(cmd_upper, "STATUS") == 0) {
          format_status_sms(resp_msg, sizeof(resp_msg));
          reply_needed = true;
        } else if (strcmp(cmd_upper, "RESTART") == 0) {
          simcom_ppp_send_sms(sender, "Reiniciando o sistema...");
          vTaskDelay(2000 / portTICK_PERIOD_MS);
          esp_restart();
        }
      }
      cJSON_Delete(json);
    }
  } else {
    char body_upper[128] = {0};
    strncpy(body_upper, body, sizeof(body_upper) - 1);
    string_to_upper(body_upper);
    
    if (strcmp(body_upper, "BUZZER") == 0) {
      stm32_cmd_send_buzzer(STM32_CMD_BUZZER_DOUBLE);
      snprintf(resp_msg, sizeof(resp_msg), "Comando BUZZER executado.");
      reply_needed = true;
    } else if (strncmp(body_upper, "RFID", 4) == 0) {
      if (strstr(body_upper, "ON") != NULL || strstr(body_upper, "1") != NULL) {
        stm32_cmd_send_rfid_power(1);
        snprintf(resp_msg, sizeof(resp_msg), "RFID ativado (ON).");
      } else if (strstr(body_upper, "OFF") != NULL || strstr(body_upper, "0") != NULL) {
        stm32_cmd_send_rfid_power(0);
        snprintf(resp_msg, sizeof(resp_msg), "RFID desativado (OFF).");
      } else {
        stm32_cmd_send_rfid_power(1);
        snprintf(resp_msg, sizeof(resp_msg), "RFID ativado.");
      }
      reply_needed = true;
    } else if (strcmp(body_upper, "STATUS") == 0) {
      format_status_sms(resp_msg, sizeof(resp_msg));
      reply_needed = true;
    } else if (strcmp(body_upper, "RESTART") == 0) {
      simcom_ppp_send_sms(sender, "Reiniciando o sistema...");
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      esp_restart();
    }
  }
  
  if (reply_needed) {
    simcom_ppp_send_sms(sender, resp_msg);
  }
  
  char del_cmd[32];
  snprintf(del_cmd, sizeof(del_cmd), "AT+CMGD=%d\r\n", index);
  at_send_cmd(del_cmd, "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);
}

esp_err_t simcom_ppp_send_sms(const char *phone_number, const char *message) {
  if (modem_state == SIMCOM_STATE_PPP_ACTIVE) {
    ESP_LOGW(TAG, "Nao e possivel enviar SMS com PPP ativo.");
    return ESP_ERR_INVALID_STATE;
  }
  
  at_send_cmd("AT+CMGF=1\r\n", "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);
  
  char cmd[64];
  snprintf(cmd, sizeof(cmd), "AT+CMGS=\"%s\"\r\n", phone_number);
  
  uart_flush_input(SIMCOM_UART_PORT);
  
  int written = uart_write_bytes(SIMCOM_UART_PORT, cmd, strlen(cmd));
  if (written < 0) {
    ESP_LOGE(TAG, "Erro ao escrever AT+CMGS na UART.");
    return ESP_FAIL;
  }
  ESP_LOGI(TAG, "SMS TX >> %s", cmd);
  
  char resp[256];
  int total_read = 0;
  uint32_t start = xTaskGetTickCount() * portTICK_PERIOD_MS;
  bool got_prompt = false;
  
  while ((xTaskGetTickCount() * portTICK_PERIOD_MS - start) < 5000) {
    int len = uart_read_bytes(SIMCOM_UART_PORT, (uint8_t *)(resp + total_read), sizeof(resp) - total_read - 1, pdMS_TO_TICKS(100));
    if (len > 0) {
      total_read += len;
      resp[total_read] = '\0';
      if (strstr(resp, ">") != NULL) {
        got_prompt = true;
        break;
      }
    }
  }
  
  if (!got_prompt) {
    ESP_LOGE(TAG, "Falha ao obter prompt '>' para SMS.");
    return ESP_FAIL;
  }
  
  uart_write_bytes(SIMCOM_UART_PORT, message, strlen(message));
  uart_write_bytes(SIMCOM_UART_PORT, "\x1a", 1);
  ESP_LOGI(TAG, "SMS Body TX >> %s", message);
  
  total_read = 0;
  start = xTaskGetTickCount() * portTICK_PERIOD_MS;
  bool success = false;
  while ((xTaskGetTickCount() * portTICK_PERIOD_MS - start) < 15000) {
    int len = uart_read_bytes(SIMCOM_UART_PORT, (uint8_t *)(resp + total_read), sizeof(resp) - total_read - 1, pdMS_TO_TICKS(100));
    if (len > 0) {
      total_read += len;
      resp[total_read] = '\0';
      if (strstr(resp, "OK") != NULL && strstr(resp, "+CMGS:") != NULL) {
        success = true;
        break;
      }
      if (strstr(resp, "ERROR") != NULL) {
        break;
      }
    }
  }
  
  if (success) {
    ESP_LOGI(TAG, "SMS enviado com sucesso para %s", phone_number);
    return ESP_OK;
  } else {
    ESP_LOGE(TAG, "Falha no envio de SMS: %s", resp);
    return ESP_FAIL;
  }
}

esp_err_t simcom_ppp_check_and_process_sms(void) {
  if (modem_state == SIMCOM_STATE_PPP_ACTIVE) {
    return ESP_ERR_INVALID_STATE;
  }

  at_send_cmd("AT+CMGF=1\r\n", "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);

  char resp[1024];
  esp_err_t err = at_send_cmd("AT+CMGL=\"REC UNREAD\"\r\n", "OK", resp, sizeof(resp), 10000);
  if (err != ESP_OK) {
    return err;
  }

  char *p = strstr(resp, "+CMGL:");
  while (p != NULL) {
    int index = -1;
    char sender[32] = {0};

    if (sscanf(p, "+CMGL: %d,\"%*[^\"]\",\"%31[^\"]\"", &index, sender) >= 2) {
      char *msg_text = strchr(p, '\n');
      if (msg_text != NULL) {
        msg_text++;

        char body[128] = {0};
        int b_idx = 0;
        while (*msg_text != '\0' && b_idx < sizeof(body) - 1) {
          if (strncmp(msg_text, "\r\n+CMGL:", 8) == 0 || strncmp(msg_text, "\r\nOK", 4) == 0) {
            break;
          }
          body[b_idx++] = *msg_text++;
        }
        body[b_idx] = '\0';

        char *trimmed_body = body;
        while (*trimmed_body == ' ' || *trimmed_body == '\r' || *trimmed_body == '\n') {
          trimmed_body++;
        }
        int len = strlen(trimmed_body);
        while (len > 0 && (trimmed_body[len - 1] == ' ' || trimmed_body[len - 1] == '\r' || trimmed_body[len - 1] == '\n')) {
          trimmed_body[len - 1] = '\0';
          len--;
        }

        process_sms_command(index, sender, trimmed_body);
      }
    }
    p = strstr(p + 6, "+CMGL:");
  }

  return ESP_OK;
}

/**
 * @brief Executa a sequencia completa de inicializacao do modem via AT.
 * @details Envia comandos AT em sequencia para verificar SIM, desabilitar
 *          eco, registrar na rede e anexar ao servico de dados GPRS.
 *
 * @return esp_err_t ESP_OK se todos os comandos de inicializacao passaram.
 */
static esp_err_t at_init_sequence(void) {
  esp_err_t err;

  // 1. Teste basico de eco — verifica que o modem esta vivo
  err = at_send_cmd("AT\r\n", "OK", NULL, 0, 2000);
  if (err != ESP_OK) {
    // Tenta novamente apos reset suave
    at_send_cmd("AT+CFUN=1,1\r\n", "OK", NULL, 0, 5000);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    err = at_send_cmd("AT\r\n", "OK", NULL, 0, 3000);
    if (err != ESP_OK) {
      ESP_LOGE(TAG,
               "Modem nao responde ao comando AT. Verifique a conexao.");
      return err;
    }
  }

  // 2. Desabilita eco de comandos
  err = at_send_cmd("ATE0\r\n", "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);
  if (err != ESP_OK) {
    return err;
  }

  // 3. Verifica status do SIM Card nos slots
  int detected_slot = -1;
  char detected_ccid[32] = "";
  bool sim_found = false;

  ESP_LOGI(TAG, "Verificando presenca de chip no slot 0...");
  at_send_cmd("AT+CDSIM=0\r\n", "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);
  vTaskDelay(3000 / portTICK_PERIOD_MS); // Aguarda reconhecimento do chip
  
  char sim_resp[AT_RESPONSE_BUF_SIZE];
  err = at_send_cmd("AT+CPIN?\r\n", "READY", sim_resp, sizeof(sim_resp), AT_DEFAULT_TIMEOUT_MS);
  if (err == ESP_OK) {
    detected_slot = 0;
    sim_found = true;
    ESP_LOGI(TAG, "Chip detectado no slot 0.");
  } else {
    ESP_LOGW(TAG, "Slot 0 sem chip ou nao pronto (Erro/Resp: %s). Tentando slot 1...", sim_resp);
    // Tenta slot 1
    at_send_cmd("AT+CDSIM=1\r\n", "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    
    err = at_send_cmd("AT+CPIN?\r\n", "READY", sim_resp, sizeof(sim_resp), AT_DEFAULT_TIMEOUT_MS);
    if (err == ESP_OK) {
      detected_slot = 1;
      sim_found = true;
      ESP_LOGI(TAG, "Chip detectado no slot 1.");
    } else {
      ESP_LOGE(TAG, "Nenhum chip detectado nos slots 0 ou 1. Resposta slot 1: %s", sim_resp);
    }
  }

  // Se detectamos um chip em algum slot, lemos o CCID
  if (sim_found) {
    err = simcom_ppp_get_ccid(detected_ccid, sizeof(detected_ccid));
    if (err == ESP_OK) {
      ESP_LOGI(TAG, "CCID lido com sucesso: %s", detected_ccid);
    } else {
      strcpy(detected_ccid, "UNKNOWN");
      ESP_LOGW(TAG, "Falha ao obter CCID do chip.");
    }
  } else {
    // Se nenhum chip foi encontrado, selecionamos o slot 0 como default
    at_send_cmd("AT+CDSIM=0\r\n", "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  // Atualiza as variaveis de status globais
  bastao_current_status.sim_present = sim_found;
  bastao_current_status.active_sim_slot = (detected_slot != -1) ? detected_slot : 0;
  strncpy(bastao_current_status.sim_ccid, detected_ccid, sizeof(bastao_current_status.sim_ccid) - 1);

  // 4. Consulta o numero IMEI para identificacao do dispositivo
  char imei_resp[AT_RESPONSE_BUF_SIZE];
  err = at_send_cmd("AT+GSN\r\n", "OK", imei_resp, sizeof(imei_resp),
                    AT_DEFAULT_TIMEOUT_MS);
  if (err == ESP_OK) {
    extract_digits(imei_resp, modem_imei, sizeof(modem_imei));
    ESP_LOGI(TAG, "IMEI do modem: %s", modem_imei);
  }

  // 5. Configura modo de reporte de registro na rede
  at_send_cmd("AT+CREG=1\r\n", "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);
  at_send_cmd("AT+CGREG=1\r\n", "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);

  modem_state = SIMCOM_STATE_AT_READY;
  ESP_LOGI(TAG, "Sequencia de inicializacao AT concluida.");
  return ESP_OK;
}

/**
 * @brief Aguarda o registro do modem na rede celular com polling.
 *
 * @param[in] timeout_ms Tempo maximo de espera pelo registro.
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
 * @details Verifica periodicamente o estado da conexao PPP e, em caso
 *          de queda, tenta reconectar automaticamente.
 *
 * @param[in] pvParameters Parametros padrao do FreeRTOS (nao utilizado).
 */
static void simcom_watchdog_task(void *pvParameters) {
  ESP_LOGI(TAG, "Watchdog de reconexao ativo. Intervalo: %d ms",
           WATCHDOG_POLL_INTERVAL_MS);

  int ppp_failures = 0;
  int active_sim_slot = 0;

  while (1) {
    vTaskDelay(WATCHDOG_POLL_INTERVAL_MS / portTICK_PERIOD_MS);

    if (cellular_suspended) {
      ESP_LOGD(TAG, "Watchdog celular: conectividade suspensa (Wi-Fi ativo).");
      continue;
    }

    if (modem_state == SIMCOM_STATE_PPP_ACTIVE) {
      // Conexao ativa — nada a fazer, limpa contador de falhas
      ppp_failures = 0;
      continue;
    }

    // PPP nao esta ativo. Se o modem responder a comandos AT, verifica SMS pendentes
    if (modem_state >= SIMCOM_STATE_AT_READY) {
      simcom_ppp_check_and_process_sms();
    }

    // Incrementa falhas de conexao consecutivas
    ppp_failures++;
    if (ppp_failures >= 3) {
      active_sim_slot = (active_sim_slot == 0) ? 1 : 0;
      ESP_LOGW(TAG, "Watchdog: %d falhas consecutivas de conexao PPP. Alternando chip SIM para slot %d...", 
               ppp_failures, active_sim_slot);
      simcom_ppp_select_sim(active_sim_slot);
      ppp_failures = 0; // Reseta para dar chance de estabelecer conexao com o novo chip
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
      ESP_LOGW(TAG, "Watchdog: Restabelecendo sessao PPP...");
      simcom_ppp_connect();
    }
  }
}

/* --- Implementacao de Funcoes Publicas --- */

esp_err_t simcom_ppp_init(void) {
  ESP_LOGI(TAG, "Inicializando driver UART para SIMCom 7663E...");

  // Se o driver ja estiver instalado (por ex. recuperacao de erro do watchdog),
  // deletamos para liberar interrupcoes, buffers e semaforos antigos.
  if (uart_is_driver_installed(SIMCOM_UART_PORT)) {
    ESP_LOGI(TAG, "Driver UART2 ja instalado. Removendo instancia antiga...");
    uart_driver_delete(SIMCOM_UART_PORT);
  }

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
    ESP_LOGE(TAG, "Falha na configuracao UART2: %d", err);
    return err;
  }

  err = uart_set_pin(SIMCOM_UART_PORT, SIMCOM_UART_TX_PIN,
                     SIMCOM_UART_RX_PIN, UART_PIN_NO_CHANGE,
                     UART_PIN_NO_CHANGE);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha na atribuicao de pinos UART2: %d", err);
    return err;
  }

  err = uart_driver_install(SIMCOM_UART_PORT, SIMCOM_UART_BUF_SIZE * 2,
                            SIMCOM_UART_BUF_SIZE, 0, NULL, 0);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha na instalacao do driver UART2: %d", err);
    return err;
  }

  // Configura o pino PWRKEY do modem SIMCom como saida digital
  gpio_config_t pwrkey_cfg = {
      .pin_bit_mask = BIT64(SIMCOM_PWRKEY_PIN),
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE
  };
  gpio_config(&pwrkey_cfg);

  // Aplica o pulso de power-up (PWRKEY) para ligar o modem celular
  ESP_LOGI(TAG, "Acionando pulso de power-up (PWRKEY) no pino GPIO%d...", SIMCOM_PWRKEY_PIN);
  gpio_set_level(SIMCOM_PWRKEY_PIN, 1);
  vTaskDelay(2000 / portTICK_PERIOD_MS); // Mantem por 2 segundos em nivel ALTO
  gpio_set_level(SIMCOM_PWRKEY_PIN, 0);

  ESP_LOGI(TAG, "UART2 ativa. TX:%d RX:%d @ %d bps. Aguardando boot completo do modem (10 segundos)...",
           SIMCOM_UART_TX_PIN, SIMCOM_UART_RX_PIN, SIMCOM_UART_BAUD_RATE);

  // Aguarda a inicializacao de boot por hardware do modem celular
  vTaskDelay(10000 / portTICK_PERIOD_MS);

  // Executa a sequencia de inicializacao AT
  err = at_init_sequence();
  if (err != ESP_OK) {
    modem_state = SIMCOM_STATE_ERROR;
    return err;
  }

  // Cria o Event Group para eventos PPP se ainda nao existir
  if (ppp_event_group == NULL) {
    ppp_event_group = xEventGroupCreate();
  }

  // Cria a tarefa FreeRTOS de recepcao PPP (prioridade 5 para escoamento rápido)
  xTaskCreate(ppp_rx_task, "ppp_rx_task", 4096, NULL, 5, NULL);

  return ESP_OK;
}

esp_err_t simcom_ppp_configure_apn(const simcom_apn_config_t *apn_config) {
  if (apn_config == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  if (modem_state < SIMCOM_STATE_AT_READY) {
    ESP_LOGE(TAG, "Modem nao esta pronto para configuracao de APN.");
    return ESP_ERR_INVALID_STATE;
  }

  // Armazena a configuracao de APN localmente para reconexoes
  memcpy(&stored_apn, apn_config, sizeof(simcom_apn_config_t));

  // 1. Aguarda o registro na rede celular
  esp_err_t err = wait_for_network_registration(NETWORK_REG_TIMEOUT_MS);
  if (err != ESP_OK) {
    return err;
  }

  // 2. Anexa ao servico de dados GPRS
  err = at_send_cmd("AT+CGATT=1\r\n", "OK", NULL, 0, 10000);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha ao anexar ao servico GPRS.");
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

  // 4. Configura autenticacao PAP/CHAP se credenciais fornecidas
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
             "Modem nao registrado. Configure a APN antes de conectar.");
    return ESP_ERR_INVALID_STATE;
  }

  ESP_LOGI(TAG, "Iniciando sessao PPP via ATD*99#...");

  // 1. Inicializa o ESP-NETIF para PPP se ainda nao existir
  if (ppp_netif == NULL) {
    esp_netif_config_t netif_ppp_config = ESP_NETIF_DEFAULT_PPP();
    ppp_netif = esp_netif_new(&netif_ppp_config);
    if (ppp_netif == NULL) {
      ESP_LOGE(TAG, "Falha ao criar a interface de rede PPP.");
      return ESP_FAIL;
    }

    // Configura e anexa o driver customizado
    ppp_driver.base.post_attach = NULL;
    
    esp_netif_driver_ifconfig_t driver_config = {
        .transmit = ppp_transmit,
        .driver_free_rx_buffer = ppp_free_rx_buffer,
        .handle = &ppp_driver
    };
    
    esp_netif_set_driver_config(ppp_netif, &driver_config);
    esp_netif_attach(ppp_netif, &ppp_driver);

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

  // Define o estado ativo para que a task RX comece a ler a serial e passar pacotes
  modem_state = SIMCOM_STATE_PPP_ACTIVE;

  // Inicia a pilha PPP do LwIP para comecar a negociacao LCP/IPCP
  esp_netif_action_start(ppp_netif, NULL, 0, NULL);

  ESP_LOGI(TAG, "Modem em modo de dados PPP. Negociando IP via LCP/IPCP...");

  // 3. Aguarda a alocacao de IP pela rede (sinalizado via on_ip_event)
  EventBits_t bits = xEventGroupWaitBits(
      ppp_event_group, PPP_CONNECTED_BIT, pdFALSE, pdTRUE,
      30000 / portTICK_PERIOD_MS);

  if (bits & PPP_CONNECTED_BIT) {
    ESP_LOGI(TAG, "Sessao PPP ativa e operacional.");
    return ESP_OK;
  }

  ESP_LOGE(TAG, "Timeout na negociacao PPP — IP nao alocado.");
  esp_netif_action_stop(ppp_netif, NULL, 0, NULL);
  modem_state = SIMCOM_STATE_ERROR;
  return ESP_ERR_TIMEOUT;
}

esp_err_t simcom_ppp_disconnect(void) {
  if (modem_state != SIMCOM_STATE_PPP_ACTIVE) {
    return ESP_OK; // Ja desconectado
  }

  ESP_LOGI(TAG, "Encerrando sessao PPP...");

  // Para a pilha PPP do LwIP para desligar a interface de rede
  esp_netif_action_stop(ppp_netif, NULL, 0, NULL);

  // Envia a sequencia de escape "+++" para sair do modo de dados
  vTaskDelay(1100 / portTICK_PERIOD_MS); // Guard time obrigatorio de 1s
  uart_write_bytes(SIMCOM_UART_PORT, "+++", 3);
  vTaskDelay(1100 / portTICK_PERIOD_MS);

  // Confirma retorno ao modo de comando
  esp_err_t err = at_send_cmd("AT\r\n", "OK", NULL, 0, 3000);
  if (err != ESP_OK) {
    ESP_LOGW(TAG, "Modem nao respondeu apos escape +++. Forcando hangup.");
    at_send_cmd("ATH\r\n", "OK", NULL, 0, 3000);
  }

  modem_state = SIMCOM_STATE_REGISTERED;
  ESP_LOGI(TAG, "Sessao PPP encerrada. Modem em modo de comandos.");
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

  // Habilita GPS se ainda nao estiver ativo
  at_send_cmd("AT+CGPS=1\r\n", "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);
  vTaskDelay(2000 / portTICK_PERIOD_MS);

  // Solicita dados de posicao
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
    // Verifica se ha dados validos (campo nao vazio)
    while (*data == ' ') data++;

    if (*data == ',' || *data == '\r' || *data == '\n') {
      ESP_LOGW(TAG, "GPS sem fix valido.");
      gps_out->valid = false;
      if (was_ppp) {
        simcom_ppp_connect();
      }
      return ESP_ERR_NOT_FOUND;
    }

    // Faz parse dos campos separados por virgula
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
    ESP_LOGI(TAG, "Conectividade celular suspensa pelo gerenciador de redundancia. Desconectando PPP...");
    simcom_ppp_disconnect();
  } else {
    ESP_LOGI(TAG, "Conectividade celular reativada pelo gerenciador de redundancia.");
  }
}

bool simcom_ppp_is_suspended(void) {
  return cellular_suspended;
}

static cellular_tech_t parse_cellular_tech(const char *tech_str) {
  if (strstr(tech_str, "LTE") || strstr(tech_str, "4G") ||
      strstr(tech_str, "E-UTRAN")) {
    return CELLULAR_TECH_4G_LTE;
  }
  if (strstr(tech_str, "NB-IoT") || strstr(tech_str, "NBIOT") ||
      strstr(tech_str, "Cat NB")) {
    return CELLULAR_TECH_NB_IOT;
  }
  if (strstr(tech_str, "WCDMA") || strstr(tech_str, "UMTS") ||
      strstr(tech_str, "HSDPA") || strstr(tech_str, "HSUPA") ||
      strstr(tech_str, "3G")) {
    return CELLULAR_TECH_3G;
  }
  if (strstr(tech_str, "GSM") || strstr(tech_str, "GPRS") ||
      strstr(tech_str, "EDGE") || strstr(tech_str, "2G")) {
    return CELLULAR_TECH_2G;
  }
  return CELLULAR_TECH_UNKNOWN;
}

esp_err_t simcom_ppp_get_signal_quality(int *rssi, int *ber) {
  if (rssi == NULL || ber == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  char resp[AT_RESPONSE_BUF_SIZE];
  esp_err_t err =
      at_send_cmd("AT+CSQ\r\n", "+CSQ:", resp, sizeof(resp), AT_DEFAULT_TIMEOUT_MS);

  if (err != ESP_OK) {
    ESP_LOGW(TAG, "Falha ao consultar CSQ: %s",
             err == ESP_ERR_TIMEOUT ? "timeout" : "erro");
    *rssi = 99;
    *ber = -1;
    return err;
  }

  int rssi_raw = 0, ber_raw = 0;
  if (sscanf(resp, "%*[^+]+CSQ: %d,%d", &rssi_raw, &ber_raw) == 2) {
    if (rssi_raw == 99) {
      *rssi = -113;
    } else {
      *rssi = -113 + (rssi_raw * 2);
    }
    *ber = (ber_raw == 99) ? -1 : ber_raw;

    ESP_LOGI(TAG, "CSQ: RSSI=%d dBm, BER=%d", *rssi, *ber);
    return ESP_OK;
  }

  ESP_LOGW(TAG, "Falha ao fazer parse do CSQ: %s", resp);
  *rssi = 99;
  *ber = -1;
  return ESP_FAIL;
}

esp_err_t simcom_ppp_get_network_info(cellular_tech_t *tech, int *mcc,
                                       int *mnc, char *operator_name) {
  if (tech == NULL || mcc == NULL || mnc == NULL || operator_name == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  *tech = CELLULAR_TECH_NONE;
  *mcc = 0;
  *mnc = 0;
  operator_name[0] = '\0';

  char resp[AT_RESPONSE_BUF_SIZE];
  esp_err_t err = at_send_cmd("AT+QNWINFO\r\n", "OK", resp, sizeof(resp),
                              AT_DEFAULT_TIMEOUT_MS);

  if (err != ESP_OK) {
    ESP_LOGW(TAG, "Falha ao consultar QNWINFO.");
    return err;
  }

  char *info = strstr(resp, "+QNWINFO:");
  if (info != NULL) {
    info += strlen("+QNWINFO:");
    while (*info == ' ') info++;

    char act_raw[32] = {0};
    int act_mcc = 0, act_mnc = 0;

    if (sscanf(info, "\"%31[^\"]\",%d,%d", act_raw, &act_mcc, &act_mnc) >= 1) {
      *tech = parse_cellular_tech(act_raw);
      *mcc = act_mcc;
      *mnc = act_mnc;
    }

    ESP_LOGI(TAG, "QNWINFO: Tech=%d (%s), MCC=%d, MNC=%d", *tech, act_raw,
             *mcc, *mnc);
  }

  char cops_resp[AT_RESPONSE_BUF_SIZE];
  err = at_send_cmd("AT+COPS?\r\n", "+COPS:", cops_resp, sizeof(cops_resp),
                    AT_DEFAULT_TIMEOUT_MS);
  if (err == ESP_OK) {
    char *cops = strstr(cops_resp, "+COPS:");
    if (cops != NULL) {
      char long_name[32] = {0};
      if (sscanf(cops, "+COPS: %*d,%*d,\"%31[^\"]\"", long_name) == 1) {
        strncpy(operator_name, long_name, 31);
        ESP_LOGI(TAG, "Operadora: %s", operator_name);
      }
    }
  }

  return ESP_OK;
}

esp_err_t simcom_ppp_get_status(cellular_status_t *status) {
  if (status == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  memset(status, 0, sizeof(cellular_status_t));
  status->modem_state = modem_state;

  if (modem_state == SIMCOM_STATE_PPP_ACTIVE) {
    // Retorna valores cacheados para evitar trafego AT na serial durante sessao PPP
    status->rssi = cached_rssi;
    status->ber = cached_ber;
    status->tech = cached_tech;
    status->mcc = cached_mcc;
    status->mnc = cached_mnc;
    strncpy(status->operator_name, cached_operator_name, sizeof(status->operator_name) - 1);
    status->registered = (cached_tech != CELLULAR_TECH_NONE);
    status->roaming = false;
    status->rsrp = cached_rsrp;
    status->rsrq = cached_rsrq;
    status->sinr = cached_sinr;
    strncpy(status->ceer, cached_ceer, sizeof(status->ceer) - 1);
  } else {
    // Quando PPP nao esta ativo, podemos consultar o modem de forma ativa
    simcom_ppp_get_signal_quality(&status->rssi, &status->ber);
    cached_rssi = status->rssi;
    cached_ber = status->ber;

    simcom_ppp_get_network_info(&status->tech, &status->mcc, &status->mnc,
                                 status->operator_name);
    cached_tech = status->tech;
    cached_mcc = status->mcc;
    cached_mnc = status->mnc;
    strncpy(cached_operator_name, status->operator_name, sizeof(cached_operator_name) - 1);

    char creg_resp[AT_RESPONSE_BUF_SIZE];
    esp_err_t err = at_send_cmd("AT+CREG?\r\n", "+CREG:", creg_resp,
                                sizeof(creg_resp), AT_DEFAULT_TIMEOUT_MS);
    if (err == ESP_OK) {
      int n, stat = 0;
      if (sscanf(creg_resp, "%*[^+]+CREG: %d,%d", &n, &stat) == 2) {
        status->registered = (stat == 1 || stat == 5);
        status->roaming = (stat == 5);
      }
    }

    // Consulta metricas especificas LTE (RSRP, RSRQ, SINR)
    query_cpsi_metrics();
    // Consulta o log de erros do modem (CEER)
    query_ceer_log();

    status->rsrp = cached_rsrp;
    status->rsrq = cached_rsrq;
    status->sinr = cached_sinr;
    strncpy(status->ceer, cached_ceer, sizeof(status->ceer) - 1);
  }

  ESP_LOGI(TAG,
           "Status Cellular: Tech=%d, RSSI=%d, Op=%s, Reg=%d, PPP=%d, RSRP=%d, RSRQ=%d, SINR=%d, CEER='%s'",
           status->tech, status->rssi, status->operator_name,
           status->registered,
           modem_state == SIMCOM_STATE_PPP_ACTIVE,
           status->rsrp, status->rsrq, status->sinr, status->ceer);

  return ESP_OK;
}

esp_err_t simcom_ppp_diagnostic_json(char *json_buf, size_t buf_size) {
  if (json_buf == NULL || buf_size == 0) {
    return ESP_ERR_INVALID_ARG;
  }

  cellular_status_t status;
  simcom_ppp_get_status(&status);

  const char *tech_str = "UNKNOWN";
  switch (status.tech) {
    case CELLULAR_TECH_2G:
      tech_str = "2G";
      break;
    case CELLULAR_TECH_3G:
      tech_str = "3G";
      break;
    case CELLULAR_TECH_4G_LTE:
      tech_str = "4G LTE";
      break;
    case CELLULAR_TECH_NB_IOT:
      tech_str = "NB-IoT";
      break;
    case CELLULAR_TECH_NONE:
      tech_str = "NONE";
      break;
    default:
      break;
  }



  int ret = snprintf(
      json_buf, buf_size,
      "{"
      "\"type\":\"cellular_diagnostic\","
      "\"rssi_dbm\":%d,"
      "\"ber\":%d,"
      "\"technology\":\"%s\","
      "\"mcc\":%d,"
      "\"mnc\":%d,"
      "\"operator\":\"%s\","
      "\"registered\":%s,"
      "\"roaming\":%s,"
      "\"modem_state\":%d,"
      "\"ppp_active\":%s"
      "}",
      status.rssi, status.ber, tech_str, status.mcc, status.mnc,
      status.operator_name, status.registered ? "true" : "false",
      status.roaming ? "true" : "false", status.modem_state,
      modem_state == SIMCOM_STATE_PPP_ACTIVE ? "true" : "false");

  if (ret < 0 || (size_t)ret >= buf_size) {
    ESP_LOGE(TAG, "Buffer insuficiente para JSON de diagnostico.");
    return ESP_ERR_NO_MEM;
  }

  ESP_LOGI(TAG, "Diagnostico celular: %s", json_buf);
  return ESP_OK;
}

esp_err_t simcom_ppp_select_sim(int slot) {
  if (slot != 0 && slot != 1) {
    return ESP_ERR_INVALID_ARG;
  }

  // Se em modo PPP, primeiro sai do modo de dados
  bool was_ppp = (modem_state == SIMCOM_STATE_PPP_ACTIVE);
  if (was_ppp) {
    simcom_ppp_disconnect();
  }

  char cmd[32];
  snprintf(cmd, sizeof(cmd), "AT+CDSIM=%d\r\n", slot);

  ESP_LOGI(TAG, "Chaveando chip SIM por software para slot %d...", slot);
  esp_err_t err = at_send_cmd(cmd, "OK", NULL, 0, AT_DEFAULT_TIMEOUT_MS);

  bool sim_found = false;
  char detected_ccid[32] = "";

  if (err == ESP_OK) {
    ESP_LOGI(TAG, "Chip SIM chaveado com sucesso para o slot %d.", slot);
    // Aguarda 3 segundos para que o modem reconheça o chip SIM chaveado
    vTaskDelay(3000 / portTICK_PERIOD_MS);

    char sim_resp[AT_RESPONSE_BUF_SIZE];
    esp_err_t pin_err = at_send_cmd("AT+CPIN?\r\n", "READY", sim_resp, sizeof(sim_resp), AT_DEFAULT_TIMEOUT_MS);
    if (pin_err == ESP_OK) {
      sim_found = true;
      esp_err_t ccid_err = simcom_ppp_get_ccid(detected_ccid, sizeof(detected_ccid));
      if (ccid_err == ESP_OK) {
        ESP_LOGI(TAG, "CCID lido no slot %d: %s", slot, detected_ccid);
      } else {
        strcpy(detected_ccid, "UNKNOWN");
        ESP_LOGW(TAG, "Falha ao obter CCID do chip no slot %d.", slot);
      }
    } else {
      ESP_LOGW(TAG, "Slot %d sem chip ou nao pronto apos selecao (Resp: %s).", slot, sim_resp);
    }

    bastao_current_status.sim_present = sim_found;
    bastao_current_status.active_sim_slot = slot;
    strncpy(bastao_current_status.sim_ccid, detected_ccid, sizeof(bastao_current_status.sim_ccid) - 1);
  } else {
    ESP_LOGE(TAG, "Falha ao chavear chip SIM para o slot %d. Erro: %d", slot, err);
  }

  // Se estava em PPP, reconecta
  if (was_ppp) {
    simcom_ppp_connect();
  }

  return err;
}

const char* simcom_ppp_get_imei(void) {
  return modem_imei;
}

