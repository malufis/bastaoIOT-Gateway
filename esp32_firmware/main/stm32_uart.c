/**
 * @file stm32_uart.c
 * @brief Implementacao do modulo de recepcao UART para comunicacao com o microcontrolador STM32.
 * @details Este arquivo contem a logica interna para o recebimento de bytes via serial,
 *          isolamento de linhas com '\n', parse estruturado de dados usando cJSON e
 *          encaminhamento das mensagens validas para a fila global do sistema.
 * 
 * @author Antigravity Agent
 * @date 2026-05-19
 */

#include "stm32_uart.h"
#include "stm32_monitor.h"
#include <string.h>
#include <stdlib.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "STM32_UART";

/* --- Definicao da Fila Global --- */
QueueHandle_t stm32_data_queue = NULL;

/* --- Watchdog do STM32 --- */
static uint32_t stm32_last_heartbeat_tick = 0;
static uint8_t stm32_watchdog_triggered = 0;

/* --- Declaracao de Funcoes Internas Privadas --- */

/**
 * @brief Realiza o parsing de uma string JSON crua recebida do STM32.
 * @details Decodifica a string usando cJSON, valida a presenca de chaves obrigatorias
 *          (como "type") e formata o dado decodificado na estrutura stm32_data_t.
 *          Caso a mensagem seja valida, ela e postada no final da fila global de dados.
 * 
 * @param[in] raw_json_str Ponteiro para a string ASCII contendo o JSON completo e finalizado com '\0'.
 * 
 * @note Esta funcao gerencia e libera internamente a memoria alocada pelo parser da biblioteca cJSON.
 * 
 * @warning Nao deve ser chamada de dentro de uma Rotina de Servico de Interrupcao (ISR) devido ao uso
 *           de cJSON_Parse (que realiza alocacao dinamica com malloc/free).
 */
static void stm32_uart_parse_json(const char *raw_json_str)
{
    if (raw_json_str == NULL || strlen(raw_json_str) == 0) {
        return;
    }

    // Tenta analisar sintaticamente a string JSON crua
    cJSON *json = cJSON_Parse(raw_json_str);
    if (json == NULL) {
        ESP_LOGE(TAG, "Falha de sintaxe ao decodificar JSON bruto: %s", raw_json_str);
        stm32_monitor_count_parse_error();
        return;
    }

    // Busca o campo chave do protocolo que dita a estrutura da mensagem
    cJSON *type_item = cJSON_GetObjectItem(json, "type");
    if (type_item == NULL || !cJSON_IsString(type_item)) {
        ESP_LOGW(TAG, "Campo obrigatorio 'type' nao encontrado ou invalido no JSON");
        cJSON_Delete(json); // Libera o objeto cJSON alocado
        return;
    }

    stm32_data_t received_data;
    memset(&received_data, 0, sizeof(stm32_data_t));

    // Logica para tratamento de tags de identificacao animal RFID
    if (strcmp(type_item->valuestring, "rfid") == 0) {
        received_data.type = DATA_TYPE_RFID;

        cJSON *model_item = cJSON_GetObjectItem(json, "model");
        cJSON *tag_item = cJSON_GetObjectItem(json, "tag");

        if (model_item != NULL && cJSON_IsString(model_item) &&
            tag_item != NULL && cJSON_IsString(tag_item)) {
            
            // Copia de forma segura prevenindo estouro de buffers estaticos
            strncpy(received_data.model, model_item->valuestring, sizeof(received_data.model) - 1);
            strncpy(received_data.tag, tag_item->valuestring, sizeof(received_data.tag) - 1);

            ESP_LOGI(TAG, "[STM32_RX]: RFID %s Tag decodificada: %s", 
                     received_data.model, received_data.tag);

            stm32_monitor_count_message("rfid", received_data.model);

            // Envia para a fila global
            if (stm32_data_queue != NULL) {
                if (xQueueSend(stm32_data_queue, &received_data, 0) != pdTRUE) {
                    ESP_LOGW(TAG, "Fila stm32_data_queue cheia. Pacote RFID descartado.");
                    stm32_monitor_count_queue_drop();
                }
            }
        } else {
            ESP_LOGW(TAG, "Campos 'model' ou 'tag' ausentes na mensagem do tipo RFID");
        }
    }
    // Logica para tratamento de dados analogicos de bateria (telemetria)
    else if (strcmp(type_item->valuestring, "batt") == 0) {
        received_data.type = DATA_TYPE_BATTERY;

        cJSON *volt_item = cJSON_GetObjectItem(json, "volt");
        if (volt_item != NULL && cJSON_IsNumber(volt_item)) {
            received_data.battery_v = (float)volt_item->valuedouble;

            ESP_LOGI(TAG, "[STM32_RX]: Telemetria de Bateria: %.2f V", received_data.battery_v);

            stm32_monitor_count_message("batt", NULL);

            // Envia para a fila global
            if (stm32_data_queue != NULL) {
                if (xQueueSend(stm32_data_queue, &received_data, 0) != pdTRUE) {
                    ESP_LOGW(TAG, "Fila stm32_data_queue cheia. Pacote de bateria descartado.");
                    stm32_monitor_count_queue_drop();
                }
            }
        } else {
            ESP_LOGW(TAG, "Campo 'volt' ausente ou invalido na mensagem do tipo bateria");
        }
    }
    else if (strcmp(type_item->valuestring, "accel") == 0) {
        received_data.type = DATA_TYPE_ACCEL;

        cJSON *x_item = cJSON_GetObjectItem(json, "x");
        cJSON *y_item = cJSON_GetObjectItem(json, "y");
        cJSON *z_item = cJSON_GetObjectItem(json, "z");
        cJSON *mov_item = cJSON_GetObjectItem(json, "movement");

        if (x_item && y_item && z_item && mov_item && cJSON_IsNumber(x_item)) {
            received_data.accel_x = (float)x_item->valuedouble;
            received_data.accel_y = (float)y_item->valuedouble;
            received_data.accel_z = (float)z_item->valuedouble;
            received_data.movement = (uint8_t)mov_item->valueint;

            ESP_LOGI(TAG, "[K10_RX]: Acel X:%.2f Y:%.2f Z:%.2f Mov:%d",
                     received_data.accel_x, received_data.accel_y,
                     received_data.accel_z, received_data.movement);

            stm32_monitor_count_message("accel", NULL);

            if (stm32_data_queue != NULL) {
                xQueueSend(stm32_data_queue, &received_data, 0);
            }
        }
    }
    else if (strcmp(type_item->valuestring, "rfid") == 0) {
        cJSON *x_item = cJSON_GetObjectItem(json, "x");
        if (x_item != NULL) {
            received_data.type = DATA_TYPE_RFID_WITH_ACCEL;
            received_data.accel_x = (float)cJSON_GetObjectItem(json, "x")->valuedouble;
            received_data.accel_y = (float)cJSON_GetObjectItem(json, "y")->valuedouble;
            received_data.accel_z = (float)cJSON_GetObjectItem(json, "z")->valuedouble;
            received_data.movement = (uint8_t)cJSON_GetObjectItem(json, "movement")->valueint;
        } else {
            received_data.type = DATA_TYPE_RFID;
        }

        cJSON *model_item = cJSON_GetObjectItem(json, "model");
        cJSON *tag_item = cJSON_GetObjectItem(json, "tag");

        if (model_item && cJSON_IsString(model_item) && tag_item && cJSON_IsString(tag_item)) {
            strncpy(received_data.model, model_item->valuestring, sizeof(received_data.model) - 1);
            strncpy(received_data.tag, tag_item->valuestring, sizeof(received_data.tag) - 1);

            ESP_LOGI(TAG, "[K10_RX]: RFID %s Tag: %s (Mov:%d)",
                     received_data.model, received_data.tag, received_data.movement);

            stm32_monitor_count_message("rfid", received_data.model);

            if (stm32_data_queue != NULL) {
                xQueueSend(stm32_data_queue, &received_data, 0);
            }
        }
    }
    else if (strcmp(type_item->valuestring, "alert") == 0) {
        received_data.type = DATA_TYPE_ALERT;

        cJSON *code_item = cJSON_GetObjectItem(json, "code");
        cJSON *volt_item = cJSON_GetObjectItem(json, "volt");

        if (code_item != NULL && cJSON_IsString(code_item)) {
            strncpy(received_data.alert_code, code_item->valuestring, sizeof(received_data.alert_code) - 1);

            if (volt_item != NULL && cJSON_IsNumber(volt_item)) {
                received_data.battery_v = (float)volt_item->valuedouble;
            }

            ESP_LOGW(TAG, "[STM32_RX]: Alerta STM32 code=%s volt=%.2f",
                     received_data.alert_code, received_data.battery_v);

            stm32_monitor_count_message("alert", NULL);

            if (stm32_data_queue != NULL) {
                xQueueSend(stm32_data_queue, &received_data, 0);
            }
        }
    }
    else if (strcmp(type_item->valuestring, "heartbeat") == 0) {
        received_data.type = DATA_TYPE_HEARTBEAT;
        stm32_last_heartbeat_tick = xTaskGetTickCount();
        stm32_watchdog_triggered = 0;

        ESP_LOGD(TAG, "[STM32_RX]: Heartbeat recebido");

        stm32_monitor_count_message("heartbeat", NULL);

        if (stm32_data_queue != NULL) {
            xQueueSend(stm32_data_queue, &received_data, 0);
        }
    }
    else {
        ESP_LOGW(TAG, "Tipo JSON nao mapeado no firmware do ESP32: %s", type_item->valuestring);
        stm32_monitor_count_unknown_type();
    }

    // Libera a memoria Heap alocada pelo parser do cJSON
    cJSON_Delete(json);
}

/**
 * @brief Task em loop infinito que realiza a leitura de bytes cruificados na UART.
 * @details Implementa um leitor que consome da UART byte a byte de forma nao bloqueante.
 *          Concatena esses bytes em um buffer local de caracteres ate encontrar o delimitador
 *          de quebra de linha ('\n' ou '\r') que simboliza a finalizacao de uma string JSON.
 * 
 * @param[in,out] pvParameters Parametros padrao do FreeRTOS. Nao utilizado neste modulo.
 */
static void stm32_uart_rx_task(void *pvParameters)
{
    // Aloca buffer para acumular caracteres da serial no heap
    uint8_t *data = (uint8_t *) malloc(STM32_UART_BUF_SIZE);
    if (data == NULL) {
        ESP_LOGE(TAG, "Falha critica de alocacao de Heap no inicio da Task");
        vTaskDelete(NULL);
        return;
    }

    int len = 0;
    ESP_LOGI(TAG, "Escuta serial ativada. Aguardando dados do STM32...");

    while (1) {
        uint8_t byte;
        // Le um unico caractere com timeout curto de 20ms
        int rxBytes = uart_read_bytes(STM32_UART_PORT, &byte, 1, 20 / portTICK_PERIOD_MS);
        
        if (rxBytes > 0) {
            stm32_monitor_count_byte();
            // Delimitador de quebra de linha detectado
            if (byte == '\n' || byte == '\r') {
                if (len > 0) {
                    data[len] = '\0'; // Garante terminacao nula para manipulacao de string C
                    stm32_uart_parse_json((char *)data);
                    len = 0; // Reseta ponteiro do acumulador
                }
            } else {
                // Acumula os bytes no buffer se nao houver estouro de capacidade
                if (len < STM32_UART_BUF_SIZE - 1) {
                    data[len++] = byte;
                } else {
                    ESP_LOGW(TAG, "Estouro de capacidade do buffer serial local. Limpando historico...");
                    stm32_monitor_count_buffer_overflow();
                    len = 0;
                }
            }
        }
    }

    free(data);
    vTaskDelete(NULL);
}

/* --- Implementacao de Funcoes Publicas --- */

esp_err_t stm32_uart_init(void)
{
    // Estrutura de configuracao padrao do periferico UART
    uart_config_t uart_config = {
        .baud_rate = STM32_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    
    // Configura os registradores do periferico selecionado (UART1)
    esp_err_t err = uart_param_config(STM32_UART_PORT, &uart_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao definir os parametros da UART. Codigo: %d", err);
        return err;
    }
    
    // Configura os pinos multiplexados de acordo com o esquema da placa (IO13=RX, IO14=TX)
    err = uart_set_pin(STM32_UART_PORT, STM32_UART_TX_PIN, STM32_UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Erro na atribuicao dos pinos fisicos da UART. Codigo: %d", err);
        return err;
    }
    
    // Instala o driver alocando buffer em anel de 2048 bytes para recepcao
    err = uart_driver_install(STM32_UART_PORT, STM32_UART_BUF_SIZE * 2, 0, 0, NULL, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Erro na instalacao e reserva do driver UART. Codigo: %d", err);
        return err;
    }
    
    stm32_monitor_init();

    ESP_LOGI(TAG, "Driver serial UART1 ativo com sucesso. Pinos RX:%d TX:%d", STM32_UART_RX_PIN, STM32_UART_TX_PIN);
    return ESP_OK;
}

BaseType_t stm32_uart_rx_task_start(UBaseType_t priority)
{
    // Cria a task FreeRTOS com pilha segura de 4KB para acomodar buffers locais e uso de cJSON
    return xTaskCreate(stm32_uart_rx_task, "stm32_uart_rx_task", 4096, NULL, priority, NULL);
}

esp_err_t stm32_uart_send_string(const char *str) {
    if (str == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    size_t len = strlen(str);
    int bytes_written = uart_write_bytes(STM32_UART_PORT, str, len);

    if (bytes_written < 0) {
        return ESP_FAIL;
    }

    return ESP_OK;
}

uint8_t stm32_uart_is_stm32_alive(void) {
    TickType_t now = xTaskGetTickCount();
    TickType_t elapsed = now - stm32_last_heartbeat_tick;

    if (elapsed > pdMS_TO_TICKS(60000)) {
        if (!stm32_watchdog_triggered) {
            stm32_watchdog_triggered = 1;
            ESP_LOGE(TAG, "WATCHDOG: STM32 sem heartbeat por >60s!");
            stm32_monitor_count_stm32_dead();
        }
        return 0;
    }

    if (stm32_last_heartbeat_tick == 0) {
        return 0;
    }

    return 1;
}

void stm32_uart_watchdog_reset(void) {
    stm32_last_heartbeat_tick = xTaskGetTickCount();
    stm32_watchdog_triggered = 0;
}
