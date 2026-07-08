/**
 * @file simcom_driver.c
 * @brief Implementacao do driver do modem celular SIMCom via comandos AT diretos.
 * @details Este modulo gerencia a comunicacao serial com o modem celular,
 *          o processamento de URCs de entrada (como mensagens MQTT recebidas)
 *          e a maquina de estados de conexao MQTT/SMS/GPS.
 *
 * @author Antigravity Agent
 * @date 2026-06-15
 */

#include "simcom_driver.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "ble_mobile.h"
#include "cmd_parser.h"
#include "cJSON.h"
#include "stm32_cmd.h"

static const char *TAG = "SIMCOM_DRV";

/* --- Variaveis Estaticas --- */

/** @brief Mutex para serializar o envio de comandos AT na UART. */
static SemaphoreHandle_t simcom_mutex = NULL;

/** @brief Semaforo para sinalizar quando a resposta a um comando AT foi concluida. */
static SemaphoreHandle_t response_sem = NULL;

/** @brief Buffer para acumular a resposta do comando AT atualmente em execucao. */
static char cmd_response_buf[SIMCOM_UART_BUF_SIZE * 2];
static int cmd_response_len = 0;
static bool awaiting_response = false;

/** @brief Estado operacional atual do modem. */
static simcom_state_t modem_state = SIMCOM_STATE_OFF;

/** @brief Flag indicando se a conectividade celular esta suspensa (Wi-Fi ativo). */
static bool cellular_suspended = false;

/** @brief Flag indicando se a conexao MQTT celular esta ativa. */
static bool cellular_mqtt_connected = false;

/** @brief IMEI lido do modem celular. */
static char modem_imei[32] = "bastao-esp-default";

/** @brief Flag indicando se o GPS foi ativado fisicamente no modem. */
static bool gps_powered_on = false;

/** @brief Flag indicando se o chip GNSS esta pronto (URC +CGNSSPWR:READY! recebido). */
static bool gnss_ready = false;

/** @brief Flag indicando se dados AGPS foram baixados com sucesso. */
static bool agps_downloaded = false;

/** @brief Semaforo para sinalizar resultado do AGPS (URC +AGPS:success./+AGPS:<err>) */
static SemaphoreHandle_t agps_sem = NULL;
static bool agps_success = false;
static int agps_error_code = 0;

/** @brief APN configurada no sistema. */
static simcom_apn_config_t stored_apn = {0};

/** @brief Handle da task de recepcao UART para evitar recriacoes. */
static TaskHandle_t simcom_rx_task_handle = NULL;

/** @brief Flag para controlar primeira inicializacao do modem (probe de chips). */
static bool first_init_done = false;

/** @brief Configuracao de MQTT ativa armazenada para reconexoes. */
static mqtt_publisher_config_t stored_mqtt_config = {0};
static bool mqtt_config_loaded = false;

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

/* --- Cache de torre celular (para geolocalizacao) --- */
static int cached_tac = 0;
static int cached_cid = 0;
static int cached_earfcn = 0;
static bool cell_tower_valid = false;

/* --- Cache de localizacao por torre celular (HTTP assincrono) --- */
static double cached_cell_lat = 0.0;
static double cached_cell_lon = 0.0;
static bool cell_tower_location_valid = false;

/* --- Prototipos de Funcoes Privadas --- */
static esp_err_t at_send_cmd(const char *cmd, const char *expected_resp,
                             char *response_buf, size_t buf_size,
                             uint32_t timeout_ms);
static esp_err_t at_send_cmd_internal(const char *cmd, const char *expected_resp,
                                      char *response_buf, size_t buf_size,
                                      uint32_t timeout_ms);
static void extract_digits(const char *src, char *dst, size_t max_len);
static esp_err_t query_cpsi_metrics(void);
static esp_err_t query_ceer_log(void);
static void process_sms_command(int index, const char *sender, const char *body);
static void process_simcom_line(const char *line);
static void simcom_uart_rx_task(void *pvParameters);
static void simcom_watchdog_task(void *pvParameters);
static esp_err_t simcom_driver_get_ccid(char *ccid_out, size_t max_len);
static esp_err_t at_init_sequence(void);
static esp_err_t wait_for_network_registration(uint32_t timeout_ms);

/* --- Implementacao do Parser e Comunicacao Serial --- */

static esp_err_t at_send_cmd_internal(const char *cmd, const char *expected_resp,
                                      char *response_buf, size_t buf_size,
                                      uint32_t timeout_ms) {
    // Drena semaforo pendente de comando anterior (se houver)
    xSemaphoreTake(response_sem, 0);

    // Limpa buffer de resposta e prepara flag
    memset(cmd_response_buf, 0, sizeof(cmd_response_buf));
    cmd_response_len = 0;
    awaiting_response = true;

    // Envia o comando
    if (cmd != NULL && strlen(cmd) > 0) {
        ESP_LOGD(TAG, "AT TX >> %s", cmd);
        ESP_LOGI(TAG, "AT TX >> %s", cmd);
        int written = uart_write_bytes(SIMCOM_UART_PORT, cmd, strlen(cmd));
        if (written < 0) {
            ESP_LOGE(TAG, "Erro ao escrever na UART do SIMCom.");
            awaiting_response = false;
            return ESP_FAIL;
        }
    }

    // Aguarda o semaforo da task RX
    esp_err_t ret = ESP_OK;
    if (xSemaphoreTake(response_sem, pdMS_TO_TICKS(timeout_ms)) != pdTRUE) {
        ESP_LOGW(TAG, "AT Timeout para comando: %s (timeout=%lums)", cmd ? cmd : "NULL", (unsigned long)timeout_ms);
        ret = ESP_ERR_TIMEOUT;
    } else {
        ESP_LOGD(TAG, "AT RX << %s", cmd_response_buf);
        
        // Se o chamador forneceu buffer, copia a resposta
        if (response_buf != NULL && buf_size > 0) {
            strncpy(response_buf, cmd_response_buf, buf_size - 1);
            response_buf[buf_size - 1] = '\0';
        }

        // Valida se contem o esperado ou falhou
        if (strstr(cmd_response_buf, "ERROR") != NULL) {
            ret = ESP_FAIL;
        } else if (expected_resp != NULL && strstr(cmd_response_buf, expected_resp) == NULL) {
            ret = ESP_FAIL;
        }
    }

    awaiting_response = false;
    ESP_LOGD(TAG, "at_send_cmd_internal: ret=%d cmd=%s", ret, cmd ? cmd : "NULL");
    return ret;
}

static esp_err_t at_send_cmd(const char *cmd, const char *expected_resp,
                             char *response_buf, size_t buf_size,
                             uint32_t timeout_ms) {
    if (simcom_mutex == NULL) {
        ESP_LOGE(TAG, "at_send_cmd: mutex NULL para cmd=%s", cmd ? cmd : "NULL");
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGD(TAG, "at_send_cmd: aguardando mutex...");
    if (xSemaphoreTake(simcom_mutex, pdMS_TO_TICKS(timeout_ms + 1000)) != pdTRUE) {
        ESP_LOGE(TAG, "Falha ao obter mutex SIMCom para o comando: %s", cmd ? cmd : "NULL");
        return ESP_ERR_TIMEOUT;
    }
    ESP_LOGD(TAG, "at_send_cmd: mutex obtido, drenando sem...");

    esp_err_t ret = at_send_cmd_internal(cmd, expected_resp, response_buf, buf_size, timeout_ms);

    xSemaphoreGive(simcom_mutex);
    return ret;
}

/* --- Estado da recepcao multilinha MQTT --- */
static int mqtt_rx_state = 0;       // 0=idle, 1=waiting_topic, 2=waiting_payload
static int mqtt_rx_topic_len = 0;
static int mqtt_rx_payload_len = 0;
static char mqtt_rx_topic[128] = {0};
static int mqtt_rx_topic_idx = 0;
static char mqtt_rx_payload[1024] = {0};
static int mqtt_rx_payload_idx = 0;

static void process_simcom_line(const char *line) {
    // 1. URCs de Mensagens MQTT Recebidas (formato multilinha: +CMQTTRXSTART/TOPIC/PAYLOAD/END)
    if (strncmp(line, "+CMQTTRXSTART:", 14) == 0) {
        int client_index = 0;
        if (sscanf(line, "+CMQTTRXSTART: %d,%d,%d", &client_index, &mqtt_rx_topic_len, &mqtt_rx_payload_len) >= 3) {
            mqtt_rx_state = 1;
            mqtt_rx_topic_idx = 0;
            mqtt_rx_payload_idx = 0;
            memset(mqtt_rx_topic, 0, sizeof(mqtt_rx_topic));
            memset(mqtt_rx_payload, 0, sizeof(mqtt_rx_payload));
        }
    }
    else if (strncmp(line, "+CMQTTRXTOPIC:", 14) == 0 && mqtt_rx_state == 1) {
        int client_index = 0, sub_topic_len = 0;
        sscanf(line, "+CMQTTRXTOPIC: %d,%d", &client_index, &sub_topic_len);
        // Le bytes do topico da UART
        uint32_t start = xTaskGetTickCount();
        while (mqtt_rx_topic_idx < sub_topic_len && (xTaskGetTickCount() - start) < pdMS_TO_TICKS(2000)) {
            uint8_t c;
            if (uart_read_bytes(SIMCOM_UART_PORT, &c, 1, pdMS_TO_TICKS(50)) > 0) {
                mqtt_rx_topic[mqtt_rx_topic_idx++] = c;
            }
        }
        mqtt_rx_topic[mqtt_rx_topic_idx] = '\0';
        mqtt_rx_state = 2;
    }
    else if (strncmp(line, "+CMQTTRXPAYLOAD:", 16) == 0 && mqtt_rx_state == 2) {
        int client_index = 0, sub_payload_len = 0;
        sscanf(line, "+CMQTTRXPAYLOAD: %d,%d", &client_index, &sub_payload_len);
        // Le bytes do payload da UART
        uint32_t start = xTaskGetTickCount();
        while (mqtt_rx_payload_idx < sub_payload_len && (xTaskGetTickCount() - start) < pdMS_TO_TICKS(2000)) {
            uint8_t c;
            if (uart_read_bytes(SIMCOM_UART_PORT, &c, 1, pdMS_TO_TICKS(50)) > 0) {
                mqtt_rx_payload[mqtt_rx_payload_idx++] = c;
            }
        }
        mqtt_rx_payload[mqtt_rx_payload_idx] = '\0';
    }
    else if (strncmp(line, "+CMQTTRXEND:", 12) == 0 && mqtt_rx_state >= 1) {
        mqtt_rx_state = 0;
        ESP_LOGI(TAG, "URC MQTT RX: Topic=%s, Payload=%.*s",
                 mqtt_rx_topic, mqtt_rx_payload_idx, mqtt_rx_payload);
        // Roteia comandos e configuracoes
        if (strstr(mqtt_rx_topic, "/cmd") != NULL) {
            cmd_parser_process_message(mqtt_rx_topic, mqtt_rx_payload);
        } else if (strstr(mqtt_rx_topic, "/config") != NULL) {
            ble_mobile_process_config_json(mqtt_rx_payload);
        }
    }
    // 2. Outras URCs do MQTT Celular
    else if (strncmp(line, "+CGNSSPWR:READY!", 15) == 0) {
        gnss_ready = true;
        ESP_LOGI(TAG, "URC GNSS: Chip GNSS pronto (+CGNSSPWR:READY!).");
    }
    else if (strncmp(line, "+AGPS:success.", 13) == 0) {
        agps_success = true;
        agps_error_code = 0;
        if (agps_sem != NULL) xSemaphoreGive(agps_sem);
        ESP_LOGI(TAG, "URC AGPS: Dados AGNSS baixados com sucesso.");
    }
    else if (strncmp(line, "+AGPS:", 6) == 0) {
        /* +AGPS:<errorcode>. — falha no download */
        agps_success = false;
        agps_error_code = atoi(line + 6);
        if (agps_sem != NULL) xSemaphoreGive(agps_sem);
        /* Decodifica erro para log detalhado */
        const char *err_desc = "desconhecido";
        switch (agps_error_code) {
            case 101: err_desc = "falha ao abrir socket"; break;
            case 102: err_desc = "falha ao obter servidor AGNSS"; break;
            case 103: err_desc = "falha ao conectar no servidor AGNSS"; break;
            case 104: err_desc = "falha ao escrever no socket"; break;
            case 105: err_desc = "falha ao ler dados AGPS do socket"; break;
            case 106: err_desc = "timeout ou servidor inacessivel"; break;
            default: break;
        }
        ESP_LOGW(TAG, "URC AGPS: Falha no download (erro=%d: %s).", agps_error_code, err_desc);
    }
    else if (strncmp(line, "+CMQTTCONNLOST:", 15) == 0) {
        ESP_LOGW(TAG, "URC MQTT: Conexao perdida com o broker.");
        cellular_mqtt_connected = false;
        modem_state = SIMCOM_STATE_REGISTERED;
        mqtt_rx_state = 0;
    }
    else if (strncmp(line, "+CMQTTNONET:", 12) == 0) {
        ESP_LOGW(TAG, "URC MQTT: Rede indisponivel (NONET). Desconectando...");
        cellular_mqtt_connected = false;
        modem_state = SIMCOM_STATE_REGISTERED;
        mqtt_rx_state = 0;
    }
    else if (strncmp(line, "+CMQTTCONNECT: 0,0", 18) == 0) {
        ESP_LOGI(TAG, "URC MQTT: Conectado com sucesso.");
        cellular_mqtt_connected = true;
        modem_state = SIMCOM_STATE_MQTT_CONNECTED;
    } else if (strncmp(line, "+CMQTTCONNECT:", 14) == 0) {
        int cid = 0, err_code = 0;
        if (sscanf(line, "+CMQTTCONNECT: %d,%d", &cid, &err_code) == 2) {
            if (err_code == 0) {
                cellular_mqtt_connected = true;
                modem_state = SIMCOM_STATE_MQTT_CONNECTED;
            } else {
                ESP_LOGE(TAG, "URC MQTT: Falha de conexao. Erro: %d", err_code);
                cellular_mqtt_connected = false;
                modem_state = SIMCOM_STATE_REGISTERED;
            }
        }
    }
    // 3. Resposta Normal de Comando AT
    else {
        if (awaiting_response) {
            int len = strlen(line);
            if (cmd_response_len + len + 2 < sizeof(cmd_response_buf)) {
                strcpy(cmd_response_buf + cmd_response_len, line);
                cmd_response_len += len;
                cmd_response_buf[cmd_response_len++] = '\n';
                cmd_response_buf[cmd_response_len] = '\0';
            }
            
            // Verifica fim de comando ou prompt
            if (strcmp(line, "OK") == 0 || strstr(line, "ERROR") != NULL || strcmp(line, ">") == 0) {
                xSemaphoreGive(response_sem);
            }
        }
    }
}

static void simcom_uart_rx_task(void *pvParameters) {
    char line_buf[512];
    int line_idx = 0;
    uint8_t c;

    ESP_LOGI(TAG, "Tarefa de recepcao serial SIMCom iniciada.");

    while (1) {
        int read_len = uart_read_bytes(SIMCOM_UART_PORT, &c, 1, portMAX_DELAY);
        if (read_len > 0) {
            if (c == '\n') {
                line_buf[line_idx] = '\0';
                // Remove CR
                if (line_idx > 0 && line_buf[line_idx - 1] == '\r') {
                    line_buf[line_idx - 1] = '\0';
                }
                if (strlen(line_buf) > 0) {
                    process_simcom_line(line_buf);
                }
                line_idx = 0;
            } else if (c == '>') {
                // Trata o prompt '>' de envio do SIMCom
                line_buf[line_idx++] = c;
                line_buf[line_idx] = '\0';
                process_simcom_line(">");
                line_idx = 0;
            } else {
                if (line_idx < sizeof(line_buf) - 2) {
                    line_buf[line_idx++] = c;
                } else {
                    line_buf[line_idx] = '\0';
                    process_simcom_line(line_buf);
                    line_idx = 0;
                }
            }
        }
    }
}

/* --- Metodos Auxiliares de Inicializacao e Rede --- */

static void extract_digits(const char *src, char *dst, size_t max_len) {
    size_t d_idx = 0;
    for (size_t s_idx = 0; src[s_idx] != '\0' && d_idx < max_len - 1; s_idx++) {
        if (src[s_idx] >= '0' && src[s_idx] <= '9') {
            dst[d_idx++] = src[s_idx];
        }
    }
    dst[d_idx] = '\0';
}

static esp_err_t simcom_driver_get_ccid(char *ccid_out, size_t max_len) {
    if (ccid_out == NULL || max_len == 0) {
        return ESP_ERR_INVALID_ARG;
    }
    ccid_out[0] = '\0';

    char resp[AT_RESPONSE_BUF_SIZE];
    esp_err_t err = at_send_cmd("AT+CICCID\r\n", "OK", resp, sizeof(resp), 5000);
    if (err != ESP_OK) {
        return err;
    }

    char *p = strstr(resp, "+CICCID:");
    if (p != NULL) {
        p += 8;
    } else {
        p = resp;
    }

    extract_digits(p, ccid_out, max_len);
    return ESP_OK;
}

static esp_err_t at_init_sequence(void) {
    esp_err_t err;

    ESP_LOGI(TAG, "[INIT] Iniciando sequencia AT (sem=%p mutex=%p)...",
             (void*)response_sem, (void*)simcom_mutex);

    // 1. Eco
    ESP_LOGI(TAG, "[INIT] Step 1: testando AT...");
    err = at_send_cmd("AT\r\n", "OK", NULL, 0, 5000);
    ESP_LOGI(TAG, "[INIT] Step 1 resultado: err=%d", err);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "[INIT] Step 1b: reset CFUN e retentativa...");
        at_send_cmd("AT+CFUN=1,1\r\n", "OK", NULL, 0, 10000);
        vTaskDelay(pdMS_TO_TICKS(5000));
        err = at_send_cmd("AT\r\n", "OK", NULL, 0, 5000);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Modem nao responde ao comando AT inicial.");
            return err;
        }
    }

    // 2. Mantem eco ativo para debug
    ESP_LOGI(TAG, "[INIT] Step 2: mantendo eco ativo (ATE1)...");
    at_send_cmd("ATE1\r\n", "OK", NULL, 0, 5000);
    ESP_LOGI(TAG, "[INIT] Step 2 concluido.");
    
    // 3. Hot swap SIM
    ESP_LOGI(TAG, "[INIT] Step 3: configurando hot swap SIM...");
    at_send_cmd("AT+UIMHOTSWAPLEVEL=0\r\n", "OK", NULL, 0, 3000);
    at_send_cmd("AT+UIMHOTSWAPON=1\r\n", "OK", NULL, 0, 3000);
    ESP_LOGI(TAG, "[INIT] Step 3 concluido.");

    // 4. Sonda ambos os slots ou escolhe o slot ativo ja conhecido
    int detected_slot = -1;
    char detected_ccid[32] = "";
    char slot0_ccid[32] = "";
    char slot1_ccid[32] = "";
    bool slot0_found = false;
    bool slot1_found = false;
    bool sim_found = false;
    int slot0_rssi = 99;
    int slot1_rssi = 99;
    char sim_resp[AT_RESPONSE_BUF_SIZE];

    if (first_init_done) {
        int active_slot = bastao_current_status.active_sim_slot;
        ESP_LOGI(TAG, "[INIT] Re-inicializacao Watchdog: Usando SIM Slot ativo %d (sem sondar)...", active_slot);
        at_send_cmd("AT+CFUN=0\r\n", "OK", NULL, 0, 5000);
        vTaskDelay(pdMS_TO_TICKS(500));
        char select_cmd[64];
        snprintf(select_cmd, sizeof(select_cmd), "AT*SELECTSIMSLOT=%d\r\n", active_slot);
        at_send_cmd(select_cmd, "OK", NULL, 0, 5000);
        vTaskDelay(pdMS_TO_TICKS(500));
        at_send_cmd("AT+CFUN=1\r\n", "OK", NULL, 0, 5000);
        vTaskDelay(pdMS_TO_TICKS(2000));

        err = at_send_cmd("AT+CPIN?\r\n", "READY", sim_resp, sizeof(sim_resp), 5000);
        if (err == ESP_OK) {
            sim_found = true;
            detected_slot = active_slot;
            simcom_driver_get_ccid(detected_ccid, sizeof(detected_ccid));
        } else {
            ESP_LOGE(TAG, "[INIT] SIM Slot %d nao respondeu READY.", active_slot);
        }
    } else {
        // --- Sonda slot 0 ---
        ESP_LOGI(TAG, "[INIT] Step 4a: sondando SIM slot 0...");
        at_send_cmd("AT+CFUN=0\r\n", "OK", NULL, 0, 5000);
        vTaskDelay(pdMS_TO_TICKS(500));
        at_send_cmd("AT*SELECTSIMSLOT=0\r\n", "OK", NULL, 0, 5000);
        vTaskDelay(pdMS_TO_TICKS(500));
        at_send_cmd("AT+CFUN=1\r\n", "OK", NULL, 0, 5000);
        vTaskDelay(pdMS_TO_TICKS(5000));

        err = at_send_cmd("AT+CPIN?\r\n", "READY", sim_resp, sizeof(sim_resp), 5000);
        if (err == ESP_OK) {
            slot0_found = true;
            simcom_driver_get_ccid(slot0_ccid, sizeof(slot0_ccid));
            // Tenta ler sinal do slot 0
            int r = 99, b = -1;
            simcom_driver_get_signal_quality(&r, &b);
            slot0_rssi = r;
            ESP_LOGI(TAG, "Slot 0: chip=OK, ccid=%s, rssi=%d dBm", slot0_ccid, slot0_rssi);
        } else {
            ESP_LOGI(TAG, "Slot 0: sem chip detectado.");
        }

        // --- Sonda slot 1 ---
        ESP_LOGI(TAG, "[INIT] Step 4b: sondando SIM slot 1...");
        at_send_cmd("AT+CFUN=0\r\n", "OK", NULL, 0, 5000);
        vTaskDelay(pdMS_TO_TICKS(500));
        at_send_cmd("AT*SELECTSIMSLOT=1\r\n", "OK", NULL, 0, 5000);
        vTaskDelay(pdMS_TO_TICKS(500));
        at_send_cmd("AT+CFUN=1\r\n", "OK", NULL, 0, 5000);
        vTaskDelay(pdMS_TO_TICKS(5000));

        err = at_send_cmd("AT+CPIN?\r\n", "READY", sim_resp, sizeof(sim_resp), 5000);
        if (err == ESP_OK) {
            slot1_found = true;
            simcom_driver_get_ccid(slot1_ccid, sizeof(slot1_ccid));
            int r = 99, b = -1;
            simcom_driver_get_signal_quality(&r, &b);
            slot1_rssi = r;
            ESP_LOGI(TAG, "Slot 1: chip=OK, ccid=%s, rssi=%d dBm", slot1_ccid, slot1_rssi);
        } else {
            ESP_LOGI(TAG, "Slot 1: sem chip detectado.");
        }

        // --- Escolhe o melhor slot ---
        if (slot0_found && slot1_found) {
            // Ambos tem chip: escolhe o de melhor sinal (> menos negativo)
            if (slot1_rssi > slot0_rssi && slot1_rssi < 30) {
                detected_slot = 1;
                strcpy(detected_ccid, slot1_ccid);
                // Ja esta no slot 1
                ESP_LOGI(TAG, "Ambos chips OK. Escolhido slot 1 (rssi=%d > slot0=%d).",
                         slot1_rssi, slot0_rssi);
            } else {
                detected_slot = 0;
                strcpy(detected_ccid, slot0_ccid);
                // Volta para slot 0
                at_send_cmd("AT+CFUN=0\r\n", "OK", NULL, 0, 5000);
                vTaskDelay(pdMS_TO_TICKS(500));
                at_send_cmd("AT*SELECTSIMSLOT=0\r\n", "OK", NULL, 0, 5000);
                vTaskDelay(pdMS_TO_TICKS(500));
                at_send_cmd("AT+CFUN=1\r\n", "OK", NULL, 0, 5000);
                vTaskDelay(pdMS_TO_TICKS(5000));
                ESP_LOGI(TAG, "Ambos chips OK. Mantido slot 0 (rssi=%d >= slot1=%d).",
                         slot0_rssi, slot1_rssi);
            }
            sim_found = true;
        } else if (slot0_found) {
            detected_slot = 0;
            strcpy(detected_ccid, slot0_ccid);
            // Volta para slot 0 (estavamos no 1 apos a sonda)
            at_send_cmd("AT+CFUN=0\r\n", "OK", NULL, 0, 5000);
            vTaskDelay(pdMS_TO_TICKS(500));
            at_send_cmd("AT*SELECTSIMSLOT=0\r\n", "OK", NULL, 0, 5000);
            vTaskDelay(pdMS_TO_TICKS(500));
            at_send_cmd("AT+CFUN=1\r\n", "OK", NULL, 0, 5000);
            vTaskDelay(pdMS_TO_TICKS(5000));
            sim_found = true;
            ESP_LOGI(TAG, "Apenas slot 0 tem chip.");
        } else if (slot1_found) {
            detected_slot = 1;
            strcpy(detected_ccid, slot1_ccid);
            // Ja esta no slot 1
            sim_found = true;
            ESP_LOGI(TAG, "Apenas slot 1 tem chip.");
        } else {
            // Nenhum chip: fallback slot 0
            ESP_LOGE(TAG, "Nenhum SIM Card detectado nos slots 0 ou 1.");
            at_send_cmd("AT+CFUN=0\r\n", "OK", NULL, 0, 5000);
            vTaskDelay(pdMS_TO_TICKS(500));
            at_send_cmd("AT*SELECTSIMSLOT=0\r\n", "OK", NULL, 0, 5000);
            vTaskDelay(pdMS_TO_TICKS(500));
            at_send_cmd("AT+CFUN=1\r\n", "OK", NULL, 0, 5000);
        }
    }

    bastao_current_status.sim_present = sim_found;
    bastao_current_status.active_sim_slot = (detected_slot != -1) ? detected_slot : 0;
    strncpy(bastao_current_status.sim_ccid, detected_ccid, sizeof(bastao_current_status.sim_ccid) - 1);

    // 5. IMEI
    char imei_resp[AT_RESPONSE_BUF_SIZE];
    if (modem_imei[0] == '\0' || strcmp(modem_imei, "bastao-esp-default") == 0) {
        ESP_LOGI(TAG, "[INIT] Step 5: lendo IMEI...");
        err = at_send_cmd("AT+GSN\r\n", "OK", imei_resp, sizeof(imei_resp), 5000);
        if (err == ESP_OK) {
            extract_digits(imei_resp, modem_imei, sizeof(modem_imei));
            strncpy(bastao_current_status.sim_imei, modem_imei, sizeof(bastao_current_status.sim_imei) - 1);
            ESP_LOGI(TAG, "IMEI do modem: %s", modem_imei);
        }
    }

    // 5b. MSISDN do chip ativo
    ESP_LOGI(TAG, "[INIT] Step 5b: lendo numero do chip...");
    char msisdn_buf[20];
    if (simcom_driver_get_msisdn(msisdn_buf, sizeof(msisdn_buf)) == ESP_OK) {
        strncpy(bastao_current_status.sim_msisdn[bastao_current_status.active_sim_slot],
                msisdn_buf, sizeof(bastao_current_status.sim_msisdn[0]) - 1);
    }

    // 6. Reportes de registro
    ESP_LOGI(TAG, "[INIT] Step 6: habilitando reportes de rede...");
    at_send_cmd("AT+CREG=1\r\n", "OK", NULL, 0, 3000);
    at_send_cmd("AT+CGREG=1\r\n", "OK", NULL, 0, 3000);

    first_init_done = true;
    modem_state = SIMCOM_STATE_AT_READY;
    ESP_LOGI(TAG, "[INIT] Sequencia AT concluida com sucesso.");
    return ESP_OK;
}

static esp_err_t wait_for_network_registration(uint32_t timeout_ms) {
    uint32_t start = xTaskGetTickCount() * portTICK_PERIOD_MS;
    char resp[AT_RESPONSE_BUF_SIZE];

    while ((xTaskGetTickCount() * portTICK_PERIOD_MS - start) < timeout_ms) {
        esp_err_t err = at_send_cmd("AT+CREG?\r\n", "+CREG:", resp, sizeof(resp), 3000);
        if (err == ESP_OK) {
            if (strstr(resp, ",1") != NULL || strstr(resp, ",5") != NULL) {
                ESP_LOGI(TAG, "Modem registrado na rede celular.");
                return ESP_OK;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
    return ESP_ERR_TIMEOUT;
}

/* --- Metodos Publicos de Rede --- */

esp_err_t simcom_driver_init(void) {
    ESP_LOGI(TAG, "Inicializando driver serial SIMCom 7663E...");
    gps_powered_on = false;
    cellular_mqtt_connected = false;

    if (simcom_mutex == NULL) {
        simcom_mutex = xSemaphoreCreateMutex();
    }
    if (response_sem == NULL) {
        response_sem = xSemaphoreCreateBinary();
    }
    if (agps_sem == NULL) {
        agps_sem = xSemaphoreCreateBinary();
    }

    if (!uart_is_driver_installed(SIMCOM_UART_PORT)) {
        uart_config_t uart_config = {
            .baud_rate = SIMCOM_UART_BAUD_RATE,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_DEFAULT,
        };

        ESP_ERROR_CHECK(uart_param_config(SIMCOM_UART_PORT, &uart_config));
        ESP_ERROR_CHECK(uart_set_pin(SIMCOM_UART_PORT, SIMCOM_UART_TX_PIN, SIMCOM_UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
        ESP_ERROR_CHECK(uart_driver_install(SIMCOM_UART_PORT, SIMCOM_UART_BUF_SIZE * 2, SIMCOM_UART_BUF_SIZE, 0, NULL, 0));
    }

    // PWRKEY GPIO setup
    gpio_config_t pwrkey_cfg = {
        .pin_bit_mask = BIT64(SIMCOM_PWRKEY_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&pwrkey_cfg);

    // Boot por hardware
    ESP_LOGI(TAG, "Pulso de boot (PWRKEY) no GPIO%d...", SIMCOM_PWRKEY_PIN);
    gpio_set_level(SIMCOM_PWRKEY_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(2000));
    gpio_set_level(SIMCOM_PWRKEY_PIN, 0);

    ESP_LOGI(TAG, "Aguardando inicializacao do hardware (10s)...");
    vTaskDelay(pdMS_TO_TICKS(10000));

    // Cria task de recepcao UART apenas se nao existir
    if (simcom_rx_task_handle == NULL) {
        xTaskCreatePinnedToCore(simcom_uart_rx_task, "simcom_rx", 4096, NULL, 6, &simcom_rx_task_handle, 1);
    } else {
        uart_flush(SIMCOM_UART_PORT);
    }

    esp_err_t err = at_init_sequence();
    if (err != ESP_OK) {
        modem_state = SIMCOM_STATE_ERROR;
        ESP_LOGE(TAG, "[INIT] Sequencia AT falhou (err=%d). SIMCom indisponivel.", err);
        return err;
    }

    // Inicializa o GPS
    simcom_driver_gps_power_on();

    ESP_LOGI(TAG, "[INIT] Driver SIMCom inicializado com sucesso.");
    return ESP_OK;
}

esp_err_t simcom_driver_configure_apn(const simcom_apn_config_t *apn_config) {
    if (apn_config == NULL) return ESP_ERR_INVALID_ARG;

    if (modem_state < SIMCOM_STATE_AT_READY) {
        ESP_LOGE(TAG, "Modem nao esta pronto.");
        return ESP_ERR_INVALID_STATE;
    }

    memcpy(&stored_apn, apn_config, sizeof(simcom_apn_config_t));

    esp_err_t err = wait_for_network_registration(60000);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha de registro na rede celular.");
        return err;
    }

    // GPRS Attach
    err = at_send_cmd("AT+CGATT=1\r\n", "OK", NULL, 0, 10000);
    if (err != ESP_OK) return err;

    // Configura PDP Context 1 com APN
    char apn_cmd[128];
    snprintf(apn_cmd, sizeof(apn_cmd), "AT+CGDCONT=1,\"IP\",\"%s\"\r\n", apn_config->apn);
    err = at_send_cmd(apn_cmd, "OK", NULL, 0, 5000);
    if (err != ESP_OK) return err;

    // Autenticacao PAP/CHAP
    if (strlen(apn_config->user) > 0) {
        char auth_cmd[256];
        snprintf(auth_cmd, sizeof(auth_cmd), "AT+CGAUTH=1,1,\"%s\",\"%s\"\r\n", apn_config->user, apn_config->password);
        at_send_cmd(auth_cmd, "OK", NULL, 0, 5000);
    }

    // Ativa PDP Context (essencial para AGPS, MQTT, etc.)
    err = at_send_cmd("AT+CGACT=1,1\r\n", "OK", NULL, 0, 15000);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Falha ao ativar PDP context 1 (CGACT). AGPS pode falhar.");
    } else {
        ESP_LOGI(TAG, "PDP Context 1 ativado com sucesso.");
    }

    modem_state = SIMCOM_STATE_REGISTERED;
    ESP_LOGI(TAG, "APN '%s' configurada e registrada com sucesso.", apn_config->apn);

    // Coleta operadora e sinal apos registro
    {
        cellular_tech_t tech = CELLULAR_TECH_NONE;
        int mcc = 0, mnc = 0;
        char oper[32] = {0};
        if (simcom_driver_get_network_info(&tech, &mcc, &mnc, oper) == ESP_OK && oper[0] != '\0') {
            int slot = bastao_current_status.active_sim_slot;
            strncpy(bastao_current_status.sim_operator[slot], oper, sizeof(bastao_current_status.sim_operator[slot]) - 1);
            ESP_LOGI(TAG, "Operadora slot %d: %s", slot, oper);
        }
        int rssi = 99, ber = -1;
        if (simcom_driver_get_signal_quality(&rssi, &ber) == ESP_OK) {
            int slot = bastao_current_status.active_sim_slot;
            bastao_current_status.sim_rssi[slot] = rssi;
            ESP_LOGI(TAG, "Sinal slot %d: %d dBm", slot, rssi);
        }
    }

    return ESP_OK;
}

/* --- Motor MQTT Embutido --- */

esp_err_t simcom_driver_mqtt_connect(const mqtt_publisher_config_t *mqtt_config) {
    if (mqtt_config == NULL) return ESP_ERR_INVALID_ARG;

    if (modem_state < SIMCOM_STATE_REGISTERED) {
        ESP_LOGE(TAG, "Modem nao registrado na rede celular.");
        return ESP_ERR_INVALID_STATE;
    }

    memcpy(&stored_mqtt_config, mqtt_config, sizeof(mqtt_publisher_config_t));
    mqtt_config_loaded = true;

    // 0. Limpa qualquer sessao MQTT anterior (se o watchdog reexecutou)
    cellular_mqtt_connected = false;
    at_send_cmd("AT+CMQTTREL=0\r\n", "OK", NULL, 0, 3000);
    at_send_cmd("AT+CMQTTSTOP\r\n", "OK", NULL, 0, 15000);
    vTaskDelay(pdMS_TO_TICKS(1000));

    // 1. Inicia o servico MQTT local no modem
    if (at_send_cmd("AT+CMQTTSTART\r\n", "OK", NULL, 0, 15000) != ESP_OK) {
        ESP_LOGE(TAG, "Erro ao iniciar servico MQTT (CMQTTSTART).");
        return ESP_FAIL;
    }

    // 2. Adquire o cliente MQTT (indice 0, ID customizado)
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "AT+CMQTTACCQ=0,\"%s\"\r\n", mqtt_config->client_id);
    if (at_send_cmd(cmd, "OK", NULL, 0, 5000) != ESP_OK) {
        ESP_LOGE(TAG, "Erro ao adquirir cliente MQTT (CMQTTACCQ).");
        return ESP_FAIL;
    }

    // 3 & 4. Configura Will Topic e Will Message sob mutex
    if (xSemaphoreTake(simcom_mutex, pdMS_TO_TICKS(15000)) == pdTRUE) {
        char will_topic[128];
        snprintf(will_topic, sizeof(will_topic), "agro/bastao/%s/status", mqtt_config->username);
        snprintf(cmd, sizeof(cmd), "AT+CMQTTWILLTOPIC=0,%d\r\n", strlen(will_topic));
        if (at_send_cmd_internal(cmd, ">", NULL, 0, 5000) == ESP_OK) {
            snprintf(cmd, sizeof(cmd), "%s\r\n", will_topic);
            at_send_cmd_internal(cmd, "OK", NULL, 0, 5000);
        }

        const char *will_msg = "offline";
        snprintf(cmd, sizeof(cmd), "AT+CMQTTWILLMSG=0,%d,1\r\n", strlen(will_msg));
        if (at_send_cmd_internal(cmd, ">", NULL, 0, 5000) == ESP_OK) {
            snprintf(cmd, sizeof(cmd), "%s\r\n", will_msg);
            at_send_cmd_internal(cmd, "OK", NULL, 0, 5000);
        }
        xSemaphoreGive(simcom_mutex);
    } else {
        ESP_LOGE(TAG, "Falha ao obter mutex para configuracao de Will Topic/Msg.");
        return ESP_ERR_TIMEOUT;
    }

    // 5. Conecta ao Broker
    // O modem suporta url no formato "tcp://broker:port"
    // Remove "mqtt://" ou prefixo se houver, ou formata diretamente
    char raw_uri[128];
    strncpy(raw_uri, mqtt_config->broker_uri, sizeof(raw_uri) - 1);
    char *clean_uri = raw_uri;
    if (strncmp(clean_uri, "mqtt://", 7) == 0) {
        clean_uri += 7;
    } else if (strncmp(clean_uri, "mqtts://", 8) == 0) {
        clean_uri += 8;
    }

    snprintf(cmd, sizeof(cmd), "AT+CMQTTCONNECT=0,\"tcp://%s\",60,1,\"%s\",\"%s\"\r\n",
             clean_uri,
             mqtt_config->username[0] ? mqtt_config->username : "bastao-esp",
             mqtt_config->password[0] ? mqtt_config->password : "");
    ESP_LOGI(TAG, "Conectando ao Broker celular: tcp://%s (user=%s)...",
             clean_uri, mqtt_config->username[0] ? mqtt_config->username : "bastao-esp");
    
    // O CMQTTCONNECT pode levar ate 30s. Ele retorna OK e gera URC
    if (at_send_cmd(cmd, "OK", NULL, 0, 30000) != ESP_OK) {
        ESP_LOGE(TAG, "Falha na solicitacao de conexao MQTT celular.");
        return ESP_FAIL;
    }

    // Aguarda o URC setar a conexao por 15 segundos adicionais
    uint32_t start = xTaskGetTickCount();
    while (!cellular_mqtt_connected && (xTaskGetTickCount() - start) < pdMS_TO_TICKS(15000)) {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    if (!cellular_mqtt_connected) {
        ESP_LOGE(TAG, "Timeout aguardando conexao MQTT URC.");
        return ESP_ERR_TIMEOUT;
    }

    // 6. Subscreve aos topicos de controle sob mutex
    if (xSemaphoreTake(simcom_mutex, pdMS_TO_TICKS(15000)) == pdTRUE) {
        // Topico de Comandos: id/MAC/cmd
        char cmd_topic[128];
        snprintf(cmd_topic, sizeof(cmd_topic), "id/%s/cmd", mqtt_config->username);
        snprintf(cmd, sizeof(cmd), "AT+CMQTTSUBTOPIC=0,%d,1\r\n", strlen(cmd_topic));
        if (at_send_cmd_internal(cmd, ">", NULL, 0, 5000) == ESP_OK) {
            snprintf(cmd, sizeof(cmd), "%s\r\n", cmd_topic);
            if (at_send_cmd_internal(cmd, "OK", NULL, 0, 5000) == ESP_OK) {
                at_send_cmd_internal("AT+CMQTTSUB=0\r\n", "OK", NULL, 0, 5000);
                ESP_LOGI(TAG, "Inscrito no topico de comando: %s", cmd_topic);
            }
        }
        xSemaphoreGive(simcom_mutex);
    } else {
        ESP_LOGE(TAG, "Falha ao obter mutex para inscricao no topico de comando.");
    }

    // Pausa para evitar erro 14 "client is busy"
    vTaskDelay(pdMS_TO_TICKS(1000));

    if (xSemaphoreTake(simcom_mutex, pdMS_TO_TICKS(15000)) == pdTRUE) {
        // Topico de Configurações: id/MAC/config
        char config_topic[128];
        snprintf(config_topic, sizeof(config_topic), "id/%s/config", mqtt_config->username);
        snprintf(cmd, sizeof(cmd), "AT+CMQTTSUBTOPIC=0,%d,1\r\n", strlen(config_topic));
        if (at_send_cmd_internal(cmd, ">", NULL, 0, 5000) == ESP_OK) {
            snprintf(cmd, sizeof(cmd), "%s\r\n", config_topic);
            if (at_send_cmd_internal(cmd, "OK", NULL, 0, 5000) == ESP_OK) {
                at_send_cmd_internal("AT+CMQTTSUB=0\r\n", "OK", NULL, 0, 5000);
                ESP_LOGI(TAG, "Inscrito no topico de configuracao: %s", config_topic);
            }
        }
        xSemaphoreGive(simcom_mutex);
    } else {
        ESP_LOGE(TAG, "Falha ao obter mutex para inscricao no topico de configuracao.");
    }

    modem_state = SIMCOM_STATE_MQTT_CONNECTED;
    return ESP_OK;
}

esp_err_t simcom_driver_mqtt_disconnect(void) {
    if (modem_state != SIMCOM_STATE_MQTT_CONNECTED) {
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Encerrando conexao MQTT celular...");
    
    // Disconnect
    at_send_cmd("AT+CMQTTDISC=0,120\r\n", "OK", NULL, 0, 5000);
    // Release
    at_send_cmd("AT+CMQTTREL=0\r\n", "OK", NULL, 0, 3000);
    // Stop MQTT
    at_send_cmd("AT+CMQTTSTOP\r\n", "OK", NULL, 0, 15000);

    cellular_mqtt_connected = false;
    modem_state = SIMCOM_STATE_REGISTERED;
    return ESP_OK;
}

esp_err_t simcom_driver_mqtt_publish(const char *topic, const char *payload, uint8_t qos) {
    if (simcom_mutex == NULL) return ESP_ERR_INVALID_STATE;

    if (modem_state != SIMCOM_STATE_MQTT_CONNECTED || !cellular_mqtt_connected) {
        ESP_LOGW(TAG, "Impossivel publicar: MQTT celular desconectado.");
        return ESP_ERR_INVALID_STATE;
    }

    // Verifica se o mutex esta disponivel (nao bloqueia se ocupado)
    // Isso evita priority inversion: se mqtt_pub_task (prio 4) nao consegue mutex,
    // significa que outra task (ex: GPS/status) esta usando o modem - vamos direto para cache
    if (xSemaphoreTake(simcom_mutex, 0) != pdTRUE) {
        ESP_LOGW(TAG, "SIMCom mutex ocupado (modem em uso). Mensagem desviada para cache offline.");
        return ESP_ERR_TIMEOUT;  // calling code desvia para cache
    }

    // Cleanup: libera message ID 0 com retry
    // Com QoS 1, o message ID 0 fica ocupado ate o PUBACK chegar.
    // Se CMQTTREL falhar (erro 14 = busy, erro 19 = ja liberado),
    // tentamos novamente ate 10x a cada 300ms (~3s totais).
    for (int retry = 0; retry < 10; retry++) {
        esp_err_t rel_err = at_send_cmd_internal("AT+CMQTTREL=0\r\n", "OK", NULL, 0, 1000);
        if (rel_err == ESP_OK) break;           // message ID 0 liberado
        if (retry < 9) vTaskDelay(pdMS_TO_TICKS(300));
    }

    esp_err_t err = ESP_OK;
    char cmd[512];

    // 1. Define o tamanho do topico
    snprintf(cmd, sizeof(cmd), "AT+CMQTTTOPIC=0,%d\r\n", strlen(topic));
    err = at_send_cmd_internal(cmd, ">", NULL, 0, 5000);
    if (err != ESP_OK) {
        xSemaphoreGive(simcom_mutex);
        return err;
    }

    // Envia o topico
    snprintf(cmd, sizeof(cmd), "%s\r\n", topic);
    err = at_send_cmd_internal(cmd, "OK", NULL, 0, 5000);
    if (err != ESP_OK) {
        xSemaphoreGive(simcom_mutex);
        return err;
    }

    // 2. Define o tamanho do payload
    snprintf(cmd, sizeof(cmd), "AT+CMQTTPAYLOAD=0,%d\r\n", strlen(payload));
    err = at_send_cmd_internal(cmd, ">", NULL, 0, 5000);
    if (err != ESP_OK) {
        xSemaphoreGive(simcom_mutex);
        return err;
    }

    // Envia o payload
    int written = uart_write_bytes(SIMCOM_UART_PORT, payload, strlen(payload));
    if (written >= 0) {
        uart_write_bytes(SIMCOM_UART_PORT, "\r\n", 2);
    }

    // Aguarda o processamento pelo modem
    char resp[128];
    err = at_send_cmd_internal("", "OK", resp, sizeof(resp), 5000);
    if (err != ESP_OK) {
        xSemaphoreGive(simcom_mutex);
        return err;
    }

    // 3. Executa a publicacao
    // NOTA: Nao faz CMQTTREL apos CMQTTPUB. Com QoS 1, o modem aguarda PUBACK
    // e message ID 0 fica busy (erro 14) ate o PUBACK chegar. O cleanup
    // e feito no inicio do proximo publish (retry loop acima).
    snprintf(cmd, sizeof(cmd), "AT+CMQTTPUB=0,%d,60\r\n", qos);
    err = at_send_cmd_internal(cmd, "OK", NULL, 0, 10000);

    xSemaphoreGive(simcom_mutex);
    return err;
}

simcom_state_t simcom_driver_get_state(void) { return modem_state; }

bool simcom_driver_is_busy(void) {
    if (simcom_mutex == NULL) return false;
    if (xSemaphoreTake(simcom_mutex, 0) == pdTRUE) {
        xSemaphoreGive(simcom_mutex);
        return false;  // mutex livre
    }
    return true;  // mutex ocupado
}

int simcom_driver_get_cached_rssi(void) {
    return cached_rssi;
}

bool simcom_driver_is_connected(void) {
    return cellular_mqtt_connected && (modem_state == SIMCOM_STATE_MQTT_CONNECTED);
}

/* --- Leitura de Localizacao (GPS/GLONASS) --- */

esp_err_t simcom_driver_get_gps(simcom_gps_data_t *gps_out) {
    if (gps_out == NULL) return ESP_ERR_INVALID_ARG;
    memset(gps_out, 0, sizeof(simcom_gps_data_t));

    ESP_LOGI(TAG, "[GPS] Solicitando leitura de localizacao...");

    if (modem_state == SIMCOM_STATE_OFF || modem_state == SIMCOM_STATE_ERROR) {
        return ESP_ERR_INVALID_STATE;
    }

    // Liga se nao estiver ativo (power ja foi ligado no boot)
    if (!gps_powered_on) {
        simcom_driver_gps_power_on();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    char gps_resp[AT_RESPONSE_BUF_SIZE];
    esp_err_t err;

    // Tenta CGPSINFO primeiro (NMEA DDMM.MMMM, resposta rapida ~200ms com fix)
    err = at_send_cmd("AT+CGPSINFO\r\n", "+CGPSINFO:", gps_resp, sizeof(gps_resp), 3000);
    if (err == ESP_OK) {
        char *data = strstr(gps_resp, "+CGPSINFO:");
        if (data != NULL) {
            data += 10;
            while (*data == ' ') data++;

            if (*data != ',' && *data != '\r' && *data != '\n') {
                double lat_raw = 0, lon_raw = 0;
                char ns = 'N', ew = 'E';
                float alt = 0, spd = 0;

                int parsed = sscanf(data, "%lf,%c,%lf,%c,%*[^,],%*[^,],%f,%f",
                                    &lat_raw, &ns, &lon_raw, &ew, &alt, &spd);

                if (parsed >= 6 && lat_raw > 0 && lon_raw > 0) {
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

                    ESP_LOGI(TAG, "[GPS] FIX OBTIDO (CGPSINFO): Lat=%.6f, Lon=%.6f, Alt=%.1fm",
                             gps_out->latitude, gps_out->longitude, gps_out->altitude);
                    return ESP_OK;
                }
            }
        }
    }

    // Fallback: tenta CGNSSINFO (so se CGPSINFO falhou, timeout curto)
    err = at_send_cmd("AT+CGNSSINFO\r\n", "+CGNSSINFO:", gps_resp, sizeof(gps_resp), 2000);
    if (err == ESP_OK) {
        char *data = strstr(gps_resp, "+CGNSSINFO:");
        if (data != NULL) {
            data += 11;
            while (*data == ' ') data++;

            if (*data != ',' && *data != '\r' && *data != '\n') {
                int fix_mode = 0;
                int gps_svs = 0, glo_svs = 0, bds_svs = 0;
                double lat_raw = 0, lon_raw = 0;
                char ns = 'N', ew = 'E';
                float alt = 0, spd = 0;

                // Formato: fix_mode,GPS_SVs,GLO_SVs,BDS_SVs,lat,N/S,lon,E/W,date,time,alt,spd,...
                int parsed = sscanf(data, "%d,%d,%d,%d,%lf,%c,%lf,%c,%*[^,],%*[^,],%f,%f",
                                    &fix_mode, &gps_svs, &glo_svs, &bds_svs,
                                    &lat_raw, &ns, &lon_raw, &ew, &alt, &spd);

                ESP_LOGI(TAG, "[GPS] CGNSSINFO parse: fix=%d sats=GPS:%d/GLO:%d/BDS:%d lat=%.4f%c lon=%.4f%c alt=%.1f",
                         fix_mode, gps_svs, glo_svs, bds_svs, lat_raw, ns, lon_raw, ew, alt);

                if (parsed >= 8 && fix_mode >= 1 && lat_raw > 0 && lon_raw > 0) {
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

                    ESP_LOGI(TAG, "[GPS] FIX OBTIDO (CGNSSINFO): Lat=%.6f, Lon=%.6f, Alt=%.1fm",
                             gps_out->latitude, gps_out->longitude, gps_out->altitude);
                    return ESP_OK;
                }
            }
        }
    }

    ESP_LOGI(TAG, "[GPS] Nenhum fix valido disponivel.");
    gps_out->valid = false;
    return ESP_ERR_NOT_FOUND;
}

esp_err_t simcom_driver_gps_power_on(void) {
    if (gps_powered_on) return ESP_OK;

    ESP_LOGI(TAG, "[GPS] Ativando receptor GNSS celular (Cold Start)...");
    gnss_ready = false;
    esp_err_t err = at_send_cmd("AT+CGNSSPWR=1\r\n", "OK", NULL, 0, 9000);
    if (err != ESP_OK) {
        err = at_send_cmd("AT+CGPS=1\r\n", "OK", NULL, 0, 9000);
    }

    if (err == ESP_OK) {
        gps_powered_on = true;
        ESP_LOGI(TAG, "[GPS] GNSS ligado com sucesso. Aguardando READY!...");
        /* Nao bloqueia aqui — gnss_ready sera setado pelo URC process_simcom_line() */
    }
    return err;
}

esp_err_t simcom_driver_gps_power_off(void) {
    if (!gps_powered_on) return ESP_OK;

    ESP_LOGI(TAG, "[GPS] Desligando receptor GNSS...");
    esp_err_t err = at_send_cmd("AT+CGNSSPWR=0\r\n", "OK", NULL, 0, 9000);
    if (err != ESP_OK) {
        err = at_send_cmd("AT+CGPS=0\r\n", "OK", NULL, 0, 9000);
    }

    if (err == ESP_OK) {
        gps_powered_on = false;
        gnss_ready = false;
        ESP_LOGI(TAG, "[GPS] GNSS desligado.");
    }
    return err;
}

/* --- Query GNSS status via AT+CGNSSPWR? --- */
static esp_err_t simcom_driver_query_gnss_status(void) {
    char resp[128] = {0};
    esp_err_t err = at_send_cmd("AT+CGNSSPWR?\r\n", "+CGNSSPWR:", resp, sizeof(resp), 5000);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "[GPS] Status GNSS: %s", resp);
    } else {
        ESP_LOGW(TAG, "[GPS] Falha ao consultar status GNSS (err=%d).", err);
    }
    return err;
}

/* Verifica se PDP Context esta ativo e retorna o IP */
static esp_err_t simcom_driver_check_pdp_context(void) {
    char resp[256] = {0};

    // Verifica se PDP context 1 esta ativo via AT+CGACT?
    esp_err_t err = at_send_cmd("AT+CGACT?\r\n", "+CGACT:", resp, sizeof(resp), 5000);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "[AGPS] Falha ao consultar CGACT. PDP context pode nao estar ativo.");
        return err;
    }

    // Resposta esperada: +CGACT: 1,1 (PDP context 1 ativo)
    if (strstr(resp, "1,1") == NULL && strstr(resp, "1,0") != NULL) {
        ESP_LOGW(TAG, "[AGPS] PDP Context 1 NAO esta ativo (resposta: %s). Tentando ativar...", resp);
        err = at_send_cmd("AT+CGACT=1,1\r\n", "OK", NULL, 0, 15000);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "[AGPS] Falha ao ativar PDP Context. AGPS nao funcionara.");
            return err;
        }
        ESP_LOGI(TAG, "[AGPS] PDP Context ativado com sucesso.");
    } else {
        ESP_LOGI(TAG, "[AGPS] PDP Context 1 ativo.");
    }

    // Verifica se temos endereco IP via AT+CGPADDR=1
    err = at_send_cmd("AT+CGPADDR=1\r\n", "+CGPADDR:", resp, sizeof(resp), 5000);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "[AGPS] Falha ao consultar endereco IP (CGPADDR). Modem pode nao ter IP.");
        return err;
    }

    // Resposta esperada: +CGPADDR: 1,"10.x.x.x" ou "100.x.x.x"
    if (strstr(resp, "\"10.") != NULL || strstr(resp, "\"100.") != NULL || strstr(resp, "\"172.") != NULL) {
        ESP_LOGI(TAG, "[AGPS] Modem possui endereco IP: %s", resp);
        return ESP_OK;
    }

    // Pode ser IP publico tambem (menos comum em 4G)
    if (strstr(resp, "\"") != NULL) {
        ESP_LOGI(TAG, "[AGPS] Modem possui endereco IP: %s", resp);
        return ESP_OK;
    }

    ESP_LOGW(TAG, "[AGPS] Modem NAO possui endereco IP (resposta: %s).", resp);
    return ESP_FAIL;
}

/* Verifica se firmware suporta XTRA (AT+CGPSXD) como alternativa ao AT+CAGPS */
static bool simcom_driver_check_xtra_support(void) {
    char resp[128] = {0};
    esp_err_t err = at_send_cmd("AT+CGPSXD=?\r\n", "OK", resp, sizeof(resp), 3000);
    if (err == ESP_OK && strstr(resp, "CGPSXD") != NULL) {
        ESP_LOGI(TAG, "[AGPS] Firmware suporta XTRA (CGPSXD). Alternativa via AT+CAGPS.");
        return true;
    }
    // Tambem tenta sem parametro
    err = at_send_cmd("AT+CGPSXD\r\n", "OK", resp, sizeof(resp), 3000);
    if (err == ESP_OK) {
        if (strstr(resp, "0") != NULL || strstr(resp, "1") != NULL) {
            ESP_LOGI(TAG, "[AGPS] Comando CGPSXD reconhecido. Suporte XTRA disponivel.");
            return true;
        }
    }
    ESP_LOGI(TAG, "[AGPS] Firmware NAO suporta XTRA (CGPSXD). Usando apenas AT+CAGPS.");
    return false;
}

esp_err_t simcom_driver_configure_gnss(void) {
    if (!gps_powered_on) {
        ESP_LOGW(TAG, "[GNSS] Nao configurado: GNSS desligado.");
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGI(TAG, "[GNSS] Configurando constelacoes (CGNSSMODE=7: GPS+BDS+GLONASS)...");
    esp_err_t err = at_send_cmd("AT+CGNSSMODE=7\r\n", "OK", NULL, 0, 5000);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "[GNSS] Constelacoes configuradas: GPS+BDS+GLONASS.");
    } else {
        ESP_LOGW(TAG, "[GNSS] CGNSSMODE=7 falhou (modem pode nao suportar). Tentando modo 3...");
        err = at_send_cmd("AT+CGNSSMODE=3\r\n", "OK", NULL, 0, 5000);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "[GNSS] Constelacoes configuradas: GPS+QZSS (modo limitado).");
        }
    }
    return err;
}

esp_err_t simcom_driver_download_agps(void) {
    if (!gps_powered_on) {
        ESP_LOGW(TAG, "[AGPS] GNSS desligado. Ligando primeiro...");
        esp_err_t e = simcom_driver_gps_power_on();
        if (e != ESP_OK) return e;
    }

    /* Aguarda chip GNSS pronto (URC +CGNSSPWR:READY!) — timeout 15s */
    /* NOTA: ASR1601 (nosso modem) NAO emite +CGNSSPWR:READY! — apenas ASR1603/1803 */
    /* Se gnss_ready nao ficar true, continuamos mesmo assim (timeout 5s e segue) */
    if (!gnss_ready) {
        ESP_LOGI(TAG, "[AGPS] Aguardando chip GNSS ficar pronto...");
        uint32_t start = xTaskGetTickCount();
        while (!gnss_ready && (xTaskGetTickCount() - start) < pdMS_TO_TICKS(5000)) {
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        if (!gnss_ready) {
            ESP_LOGW(TAG, "[AGPS] URC +CGNSSPWR:READY! nao recebido (normal em ASR1601). Continuando...");
        } else {
            ESP_LOGI(TAG, "[AGPS] Chip GNSS pronto (+CGNSSPWR:READY!).");
        }
    }

    /* Configura constelacoes (GPS+BDS+GLONASS para mais satelites) */
    simcom_driver_configure_gnss();

    /* Verifica status GNSS via comando de leitura (diagnostico) */
    simcom_driver_query_gnss_status();

    /* Verifica conectividade 4G antes do download AGPS */
    ESP_LOGI(TAG, "[AGPS] Verificando PDP Context e endereco IP...");
    if (simcom_driver_check_pdp_context() != ESP_OK) {
        ESP_LOGE(TAG, "[AGPS] SEM CONECTIVIDADE 4G! PDP Context inativo ou sem IP. AGPS nao funcionara.");
        ESP_LOGI(TAG, "[AGPS] GPS usara cold start puro. Tentando novamente quando 4G estiver ativo.");
        return ESP_ERR_INVALID_STATE;
    }

    /* Baixa dados de assistencia do servidor AGNSS via 4G */
    ESP_LOGI(TAG, "[AGPS] Baixando dados de assistencia GNSS (AT+CAGPS)...");
    ESP_LOGI(TAG, "[AGPS] NOTA: Comando pode levar ate 9s (Max ResponseTime da documentacao).");

    /* Prepara para receber URC +AGPS:success./+AGPS:<err> */
    agps_success = false;
    agps_error_code = 0;
    xSemaphoreTake(agps_sem, 0);  /* drena semaforo pendente */

    /* AT+CAGPS retorna OK imediatamente, +AGPS:success. chega como URC depois */
    /* Documentacao SIMCom: Max ResponseTime = 9000ms. Usamos 12s com margem. */
    esp_err_t err = at_send_cmd("AT+CAGPS\r\n", "OK", NULL, 0, 12000);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "[AGPS] Comando AT+CAGPS aceito. Aguardando resultado (timeout 15s)...");
        /* Aguarda URC +AGPS:success./+AGPS:<err> — timeout 15s */
        if (xSemaphoreTake(agps_sem, pdMS_TO_TICKS(15000)) == pdTRUE) {
            if (agps_success) {
                agps_downloaded = true;
                ESP_LOGI(TAG, "[AGPS] Dados AGNSS baixados com SUCESSO. GPS deve obter fix muito mais rapido.");
                /* Reinicia GPS com dados de assistencia (Cold Start com efeméride) */
                at_send_cmd("AT+CGPSCOLD\r\n", "OK", NULL, 0, 5000);
            } else {
                ESP_LOGW(TAG, "[AGPS] Falha no download AGNSS (erro=%d). GPS usara cold start puro.",
                         agps_error_code);
                ESP_LOGW(TAG, "[AGPS] NOTA: Erro 106 = timeout/servidor inacessivel. Verificar conectividade 4G.");
                /* Se erro 106, verifica se firmware usa XTRA como alternativa */
                if (agps_error_code == 106) {
                    ESP_LOGI(TAG, "[AGPS] Testando suporte a XTRA (AT+CGPSXD) como alternativa...");
                    if (simcom_driver_check_xtra_support()) {
                        ESP_LOGI(TAG, "[AGPS] Firmware suporta XTRA. Caso AGPS continue falhando, considerar usar AT+CGPSXD.");
                    }
                }
            }
        } else {
            ESP_LOGW(TAG, "[AGPS] Timeout aguardando URC +AGPS (15s). GPS usara cold start puro.");
            ESP_LOGW(TAG, "[AGPS] Possivel causa: servidor AGNSS lento ou inacessivel.");
        }
    } else {
        ESP_LOGW(TAG, "[AGPS] Comando AT+CAGPS falhou (err=%d). GPS usara cold start puro.", err);
    }

    return err;
}

/* --- Metricas e Diagnosticos Cellular --- */

static esp_err_t query_cpsi_metrics(void) {
    char resp[AT_RESPONSE_BUF_SIZE];
    esp_err_t err = at_send_cmd("AT+CPSI?\r\n", "+CPSI:", resp, sizeof(resp), 5000);
    if (err != ESP_OK) return err;

    char *p = strstr(resp, "+CPSI:");
    if (p == NULL) return ESP_FAIL;

    p += 6;
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
            int new_tac = atoi(fields[2]);
            int new_cid = atoi(fields[3]);
            cached_earfcn = atoi(fields[4]);

            /* Se a torre mudou (CID diferente), invalida cache de localizacao */
            if (new_cid != cached_cid) {
                cell_tower_location_valid = false;
                ESP_LOGD(TAG, "[CELL] Torre mudou: CID %d -> %d. Cache de localizacao invalidado.",
                         cached_cid, new_cid);
            }

            cached_tac = new_tac;
            cached_cid = new_cid;
            cell_tower_valid = (cached_cid != 0);
            cached_rsrq = atoi(fields[11]);
            cached_rsrp = atoi(fields[12]);
            int rssi_cpsi = atoi(fields[13]);
            if (rssi_cpsi != 0) {
                cached_rssi = rssi_cpsi;
            }
            cached_sinr = atoi(fields[14]);
            return ESP_OK;
        }
    }
    return ESP_FAIL;
}

/**
 * @brief Obtem localizacao approximada via torre celular (Mozilla Location Service).
 *
 * Usa MCC, MNC, TAC, CID e EARFCN para consultar a API do Mozilla.
 * Requer conexao ativa (Wi-Fi ou celular com dados).
 *
 * @param[out] lat Latitude obtida
 * @param[out] lon Longitude obtida
 * @return ESP_OK se obteve localizacao, ESP_FAIL caso contrario
 */
static esp_err_t cell_tower_get_location(double *lat, double *lon) {
    if (!cell_tower_valid || cached_cid == 0) {
        return ESP_ERR_INVALID_STATE;
    }

    /* Monta JSON para Mozilla Location Service */
    cJSON *root = cJSON_CreateObject();
    cJSON *cellests = cJSON_AddArrayToObject(root, "cellTowers");

    cJSON *cell = cJSON_CreateObject();
    cJSON_AddNumberToObject(cell, "mobileCountryCode", cached_mcc);
    cJSON_AddNumberToObject(cell, "mobileNetworkCode", cached_mnc);
    cJSON_AddNumberToObject(cell, "locationAreaCode", cached_tac);
    cJSON_AddNumberToObject(cell, "cellId", cached_cid);
    cJSON_AddNumberToObject(cell, "age", 0);
    cJSON_AddNumberToObject(cell, "signalStrength", cached_rsrp);
    cJSON_AddNumberToObject(cell, "timingAdvance", 0);
    cJSON_AddItemToArray(cellests, cell);

    char *post_data = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    if (!post_data) return ESP_FAIL;

    ESP_LOGI(TAG, "MLS Request: MCC=%d MNC=%d TAC=%d CID=%d EARFCN=%d",
             cached_mcc, cached_mnc, cached_tac, cached_cid, cached_earfcn);

    /* Buffer para resposta */
    char response_buf[512];
    memset(response_buf, 0, sizeof(response_buf));

    esp_http_client_config_t http_config = {
        .url = "https://location.services.mozilla.com/v1/geolocate?key=test",
        .method = HTTP_METHOD_POST,
        .timeout_ms = 10000,
    };

    esp_http_client_handle_t client = esp_http_client_init(&http_config);
    if (!client) {
        free(post_data);
        return ESP_FAIL;
    }

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_err_t err = esp_http_client_open(client, strlen(post_data));
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "MLS HTTP open falhou: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        free(post_data);
        return err;
    }

    int written = esp_http_client_write(client, post_data, strlen(post_data));
    free(post_data);

    if (written < 0) {
        ESP_LOGE(TAG, "MLS HTTP write falhou");
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    int read_len = esp_http_client_read(client, response_buf, sizeof(response_buf) - 1);
    esp_http_client_close(client);
    esp_http_client_cleanup(client);

    if (read_len <= 0) {
        ESP_LOGW(TAG, "MLS: Sem resposta HTTP");
        return ESP_FAIL;
    }
    response_buf[read_len] = '\0';

    /* Parse da resposta: {"location":{"lat":-23.55,"lng":-46.63},"accuracy":1500} */
    cJSON *resp_root = cJSON_Parse(response_buf);
    if (!resp_root) {
        ESP_LOGW(TAG, "MLS: JSON parse falhou");
        return ESP_FAIL;
    }

    cJSON *location = cJSON_GetObjectItem(resp_root, "location");
    if (!location) {
        cJSON_Delete(resp_root);
        ESP_LOGW(TAG, "MLS: campo 'location' ausente");
        return ESP_FAIL;
    }

    cJSON *lat_json = cJSON_GetObjectItem(location, "lat");
    cJSON *lng_json = cJSON_GetObjectItem(location, "lng");
    if (!lat_json || !lng_json) {
        cJSON_Delete(resp_root);
        ESP_LOGW(TAG, "MLS: campos lat/lng ausentes");
        return ESP_FAIL;
    }

    *lat = lat_json->valuedouble;
    *lon = lng_json->valuedouble;

    cJSON *accuracy = cJSON_GetObjectItem(resp_root, "accuracy");
    int acc = accuracy ? accuracy->valueint : -1;

    ESP_LOGI(TAG, "MLS: lat=%.6f lon=%.6f accuracy=%dm", *lat, *lon, acc);
    cJSON_Delete(resp_root);
    return ESP_OK;
}

static esp_err_t query_ceer_log(void) {
    char resp[AT_RESPONSE_BUF_SIZE];
    esp_err_t err = at_send_cmd("AT+CEER\r\n", "+CEER:", resp, sizeof(resp), 5000);
    if (err != ESP_OK) return err;

    char *p = strstr(resp, "+CEER:");
    if (p == NULL) return ESP_FAIL;

    p += 6;
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
    return ESP_OK;
}

esp_err_t simcom_driver_get_signal_quality(int *rssi, int *ber) {
    char resp[128];
    esp_err_t err = at_send_cmd("AT+CSQ\r\n", "+CSQ:", resp, sizeof(resp), 9000);
    if (err != ESP_OK) return err;

    char *p = strstr(resp, "+CSQ:");
    if (p != NULL) {
        p += 5;
        int r = 99, b = -1;
        if (sscanf(p, "%d,%d", &r, &b) == 2) {
            if (r != 99) {
                // Converte RSSI AT para dBm
                cached_rssi = -113 + (r * 2);
            } else {
                cached_rssi = 99;
            }
            cached_ber = b;
            if (rssi) *rssi = cached_rssi;
            if (ber) *ber = cached_ber;
            return ESP_OK;
        }
    }
    return ESP_FAIL;
}

esp_err_t simcom_driver_get_network_info(cellular_tech_t *tech, int *mcc,
                                         int *mnc, char *operator_name) {
    // Tenta extrair info via comando de operador
    char resp[256];
    esp_err_t err = at_send_cmd("AT+COPS?\r\n", "+COPS:", resp, sizeof(resp), 65000);
    if (err == ESP_OK) {
        char *p = strstr(resp, "+COPS:");
        if (p != NULL) {
            p += 6;
            int mode = 0, format = 0;
            char op[32] = "";
            int act = -1;
            // Formato: +COPS: <mode>,<format>,"<oper>",<act>
            int parsed = sscanf(p, "%d,%d,\"%[^\"]\",%d", &mode, &format, op, &act);
            if (parsed >= 3) {
                strncpy(cached_operator_name, op, sizeof(cached_operator_name) - 1);
                if (act == 0) cached_tech = CELLULAR_TECH_2G;
                else if (act == 2) cached_tech = CELLULAR_TECH_3G;
                else if (act == 7) cached_tech = CELLULAR_TECH_4G_LTE;
                else if (act == 9) cached_tech = CELLULAR_TECH_NB_IOT;
            }
        }
    }

    query_cpsi_metrics();

    if (tech) *tech = cached_tech;
    if (operator_name) strncpy(operator_name, cached_operator_name, 31);
    
    // Leitura MNC/MCC
    char info_resp[128];
    if (at_send_cmd("AT+CIMI\r\n", "OK", info_resp, sizeof(info_resp), 5000) == ESP_OK) {
        char cimi[32];
        extract_digits(info_resp, cimi, sizeof(cimi));
        if (strlen(cimi) >= 5) {
            char mcc_str[4] = {cimi[0], cimi[1], cimi[2], '\0'};
            char mnc_str[3] = {cimi[3], cimi[4], '\0'};
            cached_mcc = atoi(mcc_str);
            cached_mnc = atoi(mnc_str);
        }
    }

    if (mcc) *mcc = cached_mcc;
    if (mnc) *mnc = cached_mnc;
    return ESP_OK;
}

esp_err_t simcom_driver_get_status(cellular_status_t *status) {
    if (status == NULL) return ESP_ERR_INVALID_ARG;

    status->rssi = cached_rssi;
    status->ber = cached_ber;
    status->tech = cached_tech;
    status->mcc = cached_mcc;
    status->mnc = cached_mnc;
    strncpy(status->operator_name, cached_operator_name, sizeof(status->operator_name) - 1);
    status->registered = (modem_state >= SIMCOM_STATE_REGISTERED);
    status->roaming = false;
    status->tx_bytes = 0;
    status->rx_bytes = 0;
    status->modem_state = modem_state;
    status->rsrp = cached_rsrp;
    status->rsrq = cached_rsrq;
    status->sinr = cached_sinr;
    strncpy(status->ceer, cached_ceer, sizeof(status->ceer) - 1);

    return ESP_OK;
}

esp_err_t simcom_driver_diagnostic_json(char *json_buf, size_t buf_size) {
    cellular_status_t st;
    simcom_driver_get_status(&st);

    const char *tech_str = "NENHUM";
    if (st.tech == CELLULAR_TECH_2G) tech_str = "2G";
    else if (st.tech == CELLULAR_TECH_3G) tech_str = "3G";
    else if (st.tech == CELLULAR_TECH_4G_LTE) tech_str = "4G_LTE";
    else if (st.tech == CELLULAR_TECH_NB_IOT) tech_str = "NB_IOT";

    snprintf(json_buf, buf_size,
             "{\"rssi\":%d,\"ber\":%d,\"tech\":\"%s\",\"mcc\":%d,\"mnc\":%d,\"operator\":\"%s\",\"state\":%d,\"rsrp\":%d,\"rsrq\":%d,\"sinr\":%d,\"ceer\":\"%s\"}",
             st.rssi, st.ber, tech_str, st.mcc, st.mnc, st.operator_name, st.modem_state, st.rsrp, st.rsrq, st.sinr, st.ceer);

    return ESP_OK;
}

const char* simcom_driver_get_imei(void) { return modem_imei; }

esp_err_t simcom_driver_get_msisdn(char *msisdn, size_t max_len) {
    if (msisdn == NULL || max_len == 0) return ESP_ERR_INVALID_ARG;
    msisdn[0] = '\0';

    char resp[AT_RESPONSE_BUF_SIZE];
    esp_err_t err = at_send_cmd("AT+CNUM\r\n", "+CNUM:", resp, sizeof(resp), 9000);
    if (err == ESP_OK) {
        char number[32] = {0};
        if (sscanf(resp, "%*[^+]+CNUM:%*[^,],\"%31[^\"]\"", number) >= 1 ||
            sscanf(resp, "%*[^+]+CNUM:%*[^,],%31[^,\r]", number) >= 1) {
            if (number[0] != '\0') {
                strncpy(msisdn, number, max_len - 1);
                ESP_LOGI(TAG, "MSISDN lido: %s", msisdn);
                return ESP_OK;
            }
        }
    }

    // Fallback: tenta AT+CPBS="ON" + AT+CPBR=1
    ESP_LOGD(TAG, "AT+CNUM falhou. Tentando CPBS=ON + CPBR=1...");
    if (at_send_cmd("AT+CPBS=\"ON\"\r\n", "OK", NULL, 0, 5000) == ESP_OK) {
        char cpbr_resp[256];
        if (at_send_cmd("AT+CPBR=1\r\n", "+CPBR:", cpbr_resp, sizeof(cpbr_resp), 5000) == ESP_OK) {
            char number[32] = {0};
            if (sscanf(cpbr_resp, "%*[^+]+CPBR:%*d,\"%31[^\"]\"", number) >= 1) {
                if (number[0] != '\0') {
                    strncpy(msisdn, number, max_len - 1);
                    ESP_LOGI(TAG, "MSISDN lido via CPBR: %s", msisdn);
                    return ESP_OK;
                }
            }
        }
    }

    ESP_LOGD(TAG, "MSISDN nao disponivel no chip.");
    return ESP_ERR_NOT_FOUND;
}

esp_err_t simcom_driver_sync_time_from_tower(void) {
    char resp[AT_RESPONSE_BUF_SIZE];
    esp_err_t err;

    at_send_cmd("AT+CTZU=1\r\n", "OK", NULL, 0, 2000);

    err = at_send_cmd("AT+CCLK?\r\n", "+CCLK:", resp, sizeof(resp), 5000);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Falha ao ler hora da torre celular (AT+CCLK?).");
        return err;
    }

    char *p = strstr(resp, "+CCLK:");
    if (p == NULL) return ESP_ERR_NOT_FOUND;
    
    p += 6;
    while (*p == ' ' || *p == '"') p++;

    int yy, MM, dd, hh, mm, ss, tz_h;
    char tz_sign = 0;
    int parsed = sscanf(p, "%d/%d/%d,%d:%d:%d%c%d",
                        &yy, &MM, &dd, &hh, &mm, &ss, &tz_sign, &tz_h);
    if (parsed < 6) return ESP_ERR_INVALID_RESPONSE;

    struct tm tm_val = {0};
    tm_val.tm_year = yy + 100;
    tm_val.tm_mon  = MM - 1;
    tm_val.tm_mday = dd;
    tm_val.tm_hour = hh;
    tm_val.tm_min  = mm;
    tm_val.tm_sec  = ss;

    // Salva e limpa TZ para mktime nao aplicar offset local
    char *saved_tz = getenv("TZ");
    unsetenv("TZ");
    tzset();

    time_t now = mktime(&tm_val);

    // Restaura TZ
    if (saved_tz) {
        setenv("TZ", saved_tz, 1);
        tzset();
    }

    if (now < 1700000000) return ESP_ERR_INVALID_RESPONSE;

    // Converte hora local (da torre) para UTC usando o offset do AT+CCLK
    if (tz_sign != 0) {
        // tz_h do AT+CCLK é em quarter-hours (3GPP TS 27.007)
        int tz_offset_sec = (tz_h > 14) ? (tz_h * 900) : (tz_h * 3600);
        if (tz_sign == '-') tz_offset_sec = -tz_offset_sec;
        now -= tz_offset_sec; // local - (-offset) = local + offset = UTC
    }

    struct timeval tv = { .tv_sec = now, .tv_usec = 0 };
    err = settimeofday(&tv, NULL);
    if (err != 0) return ESP_FAIL;

    ESP_LOGI(TAG, "Relogio ajustado via torre celular: %04d-%02d-%02d %02d:%02d:%02d UTC",
             2000 + yy, MM, dd, hh, mm, ss);
    return ESP_OK;
}

esp_err_t simcom_driver_select_sim(int slot) {
    if (slot != 0 && slot != 1) return ESP_ERR_INVALID_ARG;

    ESP_LOGI(TAG, "Selecionando SIM Slot %d...", slot);
    
    // CFUN=0 -> Troca Slot -> CFUN=1
    at_send_cmd("AT+CFUN=0\r\n", "OK", NULL, 0, 5000);
    vTaskDelay(pdMS_TO_TICKS(500));

    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT*SELECTSIMSLOT=%d\r\n", slot);
    esp_err_t err = at_send_cmd(cmd, "OK", NULL, 0, 5000);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao selecionar SIM Slot %d.", slot);
        at_send_cmd("AT+CFUN=1\r\n", "OK", NULL, 0, 5000);
        return err;
    }

    vTaskDelay(pdMS_TO_TICKS(500));
    at_send_cmd("AT+CFUN=1\r\n", "OK", NULL, 0, 5000);
    vTaskDelay(pdMS_TO_TICKS(5000)); // Espera registro inicial

    char sim_resp[128];
    err = at_send_cmd("AT+CPIN?\r\n", "READY", sim_resp, sizeof(sim_resp), 5000);
    bool sim_found = (err == ESP_OK);

    char detected_ccid[32] = "";
    if (sim_found) {
        simcom_driver_get_ccid(detected_ccid, sizeof(detected_ccid));
    } else {
        strcpy(detected_ccid, "UNKNOWN");
    }

    bastao_current_status.sim_present = sim_found;
    bastao_current_status.active_sim_slot = sim_found ? slot : bastao_current_status.active_sim_slot;
    strncpy(bastao_current_status.sim_ccid, detected_ccid, sizeof(bastao_current_status.sim_ccid) - 1);

    if (sim_found) {
        modem_state = SIMCOM_STATE_AT_READY;
        if (stored_apn.apn[0] != '\0') {
            simcom_driver_configure_apn(&stored_apn);
        }
        return ESP_OK;
    }

    return ESP_FAIL;
}

/* --- SMS de Contingencia --- */

static void process_sms_command(int index, const char *sender, const char *body) {
    ESP_LOGI(TAG, "SMS Comando Recebido: %s de %s (index %d)", body, sender, index);
    
    char resp_msg[256] = {0};
    bool reply_needed = false;
    char body_upper[128] = {0};
    strncpy(body_upper, body, sizeof(body_upper) - 1);
    
    // Converte para upper case
    for (int i = 0; body_upper[i]; i++) {
        if (body_upper[i] >= 'a' && body_upper[i] <= 'z') {
            body_upper[i] = body_upper[i] - 'a' + 'A';
        }
    }

    if (strcmp(body_upper, "BUZZER") == 0) {
        stm32_cmd_send_buzzer(STM32_CMD_BUZZER_DOUBLE);
        snprintf(resp_msg, sizeof(resp_msg), "Buzzer acionado com sucesso.");
        reply_needed = true;
    } else if (strncmp(body_upper, "RFID", 4) == 0) {
        if (strstr(body_upper, "ON") != NULL || strstr(body_upper, "1") != NULL) {
            stm32_cmd_send_rfid_power(1);
            snprintf(resp_msg, sizeof(resp_msg), "RFID ligado.");
        } else {
            stm32_cmd_send_rfid_power(0);
            snprintf(resp_msg, sizeof(resp_msg), "RFID desligado.");
        }
        reply_needed = true;
    } else if (strcmp(body_upper, "STATUS") == 0) {
        snprintf(resp_msg, sizeof(resp_msg),
                 "Bat:%.2fV, Cell:%s, Tags:%lu, SIM Slot:%d, GPS:%s (Lat:%.6f, Lon:%.6f)",
                 bastao_current_status.battery_voltage,
                 cellular_mqtt_connected ? "MQTT_OK" : "NO_MQTT",
                 (unsigned long)bastao_current_status.tags_read_count,
                 bastao_current_status.active_sim_slot,
                 bastao_current_status.gps_fix ? "OK" : "NO_FIX",
                 bastao_current_status.gps_latitude,
                 bastao_current_status.gps_longitude);
        reply_needed = true;
    } else if (strcmp(body_upper, "RESTART") == 0) {
        simcom_driver_send_sms(sender, "Reiniciando ESP32...");
        vTaskDelay(pdMS_TO_TICKS(2000));
        esp_restart();
    }

    if (reply_needed) {
        simcom_driver_send_sms(sender, resp_msg);
    }

    // Deleta o SMS para evitar encher a memoria do SIM
    char del_cmd[32];
    snprintf(del_cmd, sizeof(del_cmd), "AT+CMGD=%d\r\n", index);
    at_send_cmd(del_cmd, "OK", NULL, 0, 5000);
}

esp_err_t simcom_driver_send_sms(const char *phone_number, const char *message) {
    at_send_cmd("AT+CMGF=1\r\n", "OK", NULL, 0, 3000);

    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+CMGS=\"%s\"\r\n", phone_number);
    
    if (at_send_cmd(cmd, ">", NULL, 0, 5000) != ESP_OK) {
        return ESP_FAIL;
    }

    // Escreve mensagem e envia Ctrl+Z (0x1A)
    uart_write_bytes(SIMCOM_UART_PORT, message, strlen(message));
    uart_write_bytes(SIMCOM_UART_PORT, "\x1a", 1);
    
    char resp[128];
    if (at_send_cmd("", "+CMGS:", resp, sizeof(resp), 15000) == ESP_OK) {
        ESP_LOGI(TAG, "SMS enviado com sucesso.");
        return ESP_OK;
    }
    
    return ESP_FAIL;
}

esp_err_t simcom_driver_check_and_process_sms(void) {
    at_send_cmd("AT+CMGF=1\r\n", "OK", NULL, 0, 3000);

    char resp[1024];
    // List unread messages
    esp_err_t err = at_send_cmd("AT+CMGL=\"REC UNREAD\"\r\n", "OK", resp, sizeof(resp), 8000);
    if (err != ESP_OK) return err;

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
                
                // Trim spaces/CR/LF
                char *trimmed = body;
                while (*trimmed == ' ' || *trimmed == '\r' || *trimmed == '\n') trimmed++;
                int len = strlen(trimmed);
                while (len > 0 && (trimmed[len - 1] == ' ' || trimmed[len - 1] == '\r' || trimmed[len - 1] == '\n')) {
                    trimmed[len - 1] = '\0';
                    len--;
                }
                
                process_sms_command(index, sender, trimmed);
            }
        }
        p = strstr(p + 6, "+CMGL:");
    }
    return ESP_OK;
}

/* --- Watchdog e Orquestracao de Conectividade --- */

static void simcom_watchdog_task(void *pvParameters) {
    ESP_LOGI(TAG, "Watchdog de conexao celular iniciado.");
    
    TickType_t connection_attempt_start = 0;
    TickType_t modem_offline_until = 0;
    bool modem_forced_off = false;
    TickType_t sim_status_tick = 0;
    int signal_fail_count = 0;
    uint32_t net_info_counter = 0;

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));

        if (cellular_suspended) {
            continue;
        }

        if (modem_forced_off) {
            if (xTaskGetTickCount() < modem_offline_until) {
                continue;
            }
            ESP_LOGI(TAG, "Watchdog: Fim do repouso forcado do modem. Religando...");
            modem_forced_off = false;
            gps_powered_on = false;
            cellular_mqtt_connected = false;
            connection_attempt_start = xTaskGetTickCount();
            if (simcom_driver_init() == ESP_OK) {
                if (stored_apn.apn[0] != '\0') {
                    if (simcom_driver_configure_apn(&stored_apn) == ESP_OK) {
                        /* Re-baixa dados AGPS apos reconexao 4G */
                        simcom_driver_download_agps();
                        if (mqtt_config_loaded) {
                            simcom_driver_mqtt_connect(&stored_mqtt_config);
                        }
                    }
                }
            }
            continue;
        }

        // Conexao estavel
        if (cellular_mqtt_connected && modem_state == SIMCOM_STATE_MQTT_CONNECTED) {
            connection_attempt_start = 0;
            // Processamento periodico de SMS de contingencia
            simcom_driver_check_and_process_sms();
            continue;
        }

        // Recupera timers de tentativa de conexao
        if (connection_attempt_start == 0) {
            connection_attempt_start = xTaskGetTickCount();
        }

        TickType_t elapsed = (xTaskGetTickCount() - connection_attempt_start) * portTICK_PERIOD_MS / 1000;
        if (elapsed >= 180) { // 3 minutos sem conseguir conectar MQTT
            ESP_LOGW(TAG, "Watchdog: 180s sem conexao MQTT. Ciclo de energia forçado de 60s...");
            at_send_cmd("AT+CPOWD=1\r\n", "OK", NULL, 0, 3000);
            modem_state = SIMCOM_STATE_OFF;
            gps_powered_on = false;
            cellular_mqtt_connected = false;
            modem_forced_off = true;
            modem_offline_until = xTaskGetTickCount() + pdMS_TO_TICKS(60000);
            connection_attempt_start = 0;
            continue;
        }

        // Log de status do SIM a cada 60s
        if (xTaskGetTickCount() - sim_status_tick > pdMS_TO_TICKS(60000)) {
            sim_status_tick = xTaskGetTickCount();
            ESP_LOGI(TAG, "Watchdog Status SIM: slot=%d, presente=%d, ccid=%s",
                     bastao_current_status.active_sim_slot,
                     bastao_current_status.sim_present,
                     bastao_current_status.sim_ccid[0] ? bastao_current_status.sim_ccid : "N/A");
        }

        // Se SIM ausente, faz swap
        if (!bastao_current_status.sim_present) {
            int other = (bastao_current_status.active_sim_slot == 0) ? 1 : 0;
            ESP_LOGI(TAG, "Watchdog: SIM ausente slot %d. Trocando para slot %d...",
                     bastao_current_status.active_sim_slot, other);
            simcom_driver_select_sim(other);
            connection_attempt_start = xTaskGetTickCount();
            signal_fail_count = 0;
            continue;
        }

        // Verifica qualidade do sinal do slot atual
        {
            int rssi = 99, ber = -1;
            if (simcom_driver_get_signal_quality(&rssi, &ber) == ESP_OK) {
                int slot = bastao_current_status.active_sim_slot;
                bastao_current_status.sim_rssi[slot] = rssi;

                // Verifica roaming via CREG
                char creg[128];
                if (at_send_cmd("AT+CREG?\r\n", "+CREG:", creg, sizeof(creg), 3000) == ESP_OK) {
                    if (strstr(creg, ",5)") != NULL || strstr(creg, ",5\r") != NULL || strstr(creg, ",5\n") != NULL) {
                        ESP_LOGW(TAG, "Watchdog: Slot %d em roaming (CREG stat=5). Sinal=%d dBm", slot, rssi);
                    }
                }

                // Sinal muito baixo: acumula contador de falha
                if (rssi < -100) { // -100dBm ou pior = praticamente sem sinal
                    signal_fail_count++;
                    ESP_LOGW(TAG, "Watchdog: Sinal baixo slot %d (rssi=%ddBm). Fail=%d/5",
                             slot, rssi, signal_fail_count);
                } else {
                    signal_fail_count = 0; // reset se sinal voltou
                }
            }
        }

        // Troca de chip por sinal baixo persistente
        if (signal_fail_count >= 5) {
            int other = (bastao_current_status.active_sim_slot == 0) ? 1 : 0;
            ESP_LOGW(TAG, "Watchdog: Sinal baixo persistente slot %d. Trocando para slot %d...",
                     bastao_current_status.active_sim_slot, other);
            signal_fail_count = 0;
            simcom_driver_select_sim(other);
            // Reconfigura APN + MQTT no novo slot
            if (stored_apn.apn[0] != '\0') {
                simcom_driver_configure_apn(&stored_apn);
            }
            if (cellular_mqtt_connected || mqtt_config_loaded) {
                cellular_mqtt_connected = false;
                if (mqtt_config_loaded) {
                    simcom_driver_mqtt_connect(&stored_mqtt_config);
                }
            }
            connection_attempt_start = xTaskGetTickCount();
            continue;
        }

        // Recuperacao da Maquina de Estados
        if (modem_state == SIMCOM_STATE_OFF || modem_state == SIMCOM_STATE_ERROR) {
            if (simcom_driver_init() != ESP_OK) {
                continue;
            }
        }

        // Processa SMS mesmo se nao conectado ao MQTT
        if (modem_state >= SIMCOM_STATE_AT_READY) {
            simcom_driver_check_and_process_sms();
        }

        if (modem_state == SIMCOM_STATE_AT_READY) {
            if (stored_apn.apn[0] != '\0') {
                simcom_driver_configure_apn(&stored_apn);
            }
        }

        if (modem_state == SIMCOM_STATE_REGISTERED) {
            if (mqtt_config_loaded) {
                simcom_driver_mqtt_connect(&stored_mqtt_config);
            }
        }

        // Atualiza informacoes lentas de rede a cada 5 minutos (30 iteracoes * 10s = 300s)
        net_info_counter++;
        if (net_info_counter >= 30 || cached_operator_name[0] == '\0') {
            net_info_counter = 0;
            if (modem_state >= SIMCOM_STATE_REGISTERED) {
                simcom_driver_get_network_info(NULL, NULL, NULL, NULL);
                query_ceer_log();
            }
        }
    }
}

BaseType_t simcom_driver_watchdog_start(UBaseType_t priority) {
    return xTaskCreatePinnedToCore(simcom_watchdog_task, "simcom_wd", 6144, NULL, priority, NULL, 1);
}

void simcom_driver_set_suspended(bool suspend) {
    cellular_suspended = suspend;
    if (suspend) {
        ESP_LOGI(TAG, "Conectividade celular suspensa (Wi-Fi ativa). Desconectando MQTT...");
        simcom_driver_mqtt_disconnect();
    } else {
        ESP_LOGI(TAG, "Conectividade celular retomada.");
    }
}

bool simcom_driver_is_suspended(void) { return cellular_suspended; }

bool simcom_driver_has_location(void) {
    /* Localizacao disponivel: GPS com fix OU torre celular com localizacao em cache */
    return bastao_current_status.gps_fix || cell_tower_location_valid;
}

esp_err_t simcom_driver_get_cell_tower_location(double *lat, double *lon) {
    /* Retorna valor em cache (non-blocking). Cache e atualizado periodicamente. */
    if (!cell_tower_location_valid || !cell_tower_valid) {
        return ESP_ERR_NOT_FOUND;
    }
    *lat = cached_cell_lat;
    *lon = cached_cell_lon;
    return ESP_OK;
}

bool simcom_driver_is_cell_tower_valid(void) {
    return cell_tower_valid;
}

esp_err_t simcom_driver_update_cell_tower_cache(void) {
    /* Nao faz nada se nao tem torre valida ou se GPS ja tem fix */
    if (!cell_tower_valid || bastao_current_status.gps_fix) {
        return ESP_OK;
    }

    double lat = 0.0, lon = 0.0;
    esp_err_t err = cell_tower_get_location(&lat, &lon);
    if (err == ESP_OK) {
        cached_cell_lat = lat;
        cached_cell_lon = lon;
        cell_tower_location_valid = true;
        ESP_LOGI(TAG, "[CELL_CACHE] Localizacao torre celular atualizada: %.6f, %.6f", lat, lon);
    }
    return err;
}
