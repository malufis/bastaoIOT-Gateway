/**
 * @file stm32_uart.c
 * @brief Implementação do módulo de recepção UART para comunicação com o microcontrolador STM32.
 * @details Este arquivo contém a lógica interna para o recebimento de bytes via serial,
 *          isolamento de linhas com '\n', parse estruturado de dados usando cJSON e
 *          encaminhamento das mensagens válidas para a fila global do sistema.
 * 
 * @author Antigravity Agent
 * @date 2026-05-19
 */

#include "stm32_uart.h"
#include <string.h>
#include <stdlib.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "STM32_UART";

/* --- Definição da Fila Global --- */
QueueHandle_t stm32_data_queue = NULL;

/* --- Declaração de Funções Internas Privadas --- */

/**
 * @brief Realiza o parsing de uma string JSON crua recebida do STM32.
 * @details Decodifica a string usando cJSON, valida a presença de chaves obrigatórias
 *          (como "type") e formata o dado decodificado na estrutura stm32_data_t.
 *          Caso a mensagem seja válida, ela é postada no final da fila global de dados.
 * 
 * @param[in] raw_json_str Ponteiro para a string ASCII contendo o JSON completo e finalizado com '\0'.
 * 
 * @note Esta função gerencia e libera internamente a memória alocada pelo parser da biblioteca cJSON.
 * 
 * @warning Não deve ser chamada de dentro de uma Rotina de Serviço de Interrupção (ISR) devido ao uso
 *           de cJSON_Parse (que realiza alocação dinâmica com malloc/free).
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
        return;
    }

    // Busca o campo chave do protocolo que dita a estrutura da mensagem
    cJSON *type_item = cJSON_GetObjectItem(json, "type");
    if (type_item == NULL || !cJSON_IsString(type_item)) {
        ESP_LOGW(TAG, "Campo obrigatório 'type' não encontrado ou inválido no JSON");
        cJSON_Delete(json); // Libera o objeto cJSON alocado
        return;
    }

    stm32_data_t received_data;
    memset(&received_data, 0, sizeof(stm32_data_t));

    // Lógica para tratamento de tags de identificação animal RFID
    if (strcmp(type_item->valuestring, "rfid") == 0) {
        received_data.type = DATA_TYPE_RFID;

        cJSON *model_item = cJSON_GetObjectItem(json, "model");
        cJSON *tag_item = cJSON_GetObjectItem(json, "tag");

        if (model_item != NULL && cJSON_IsString(model_item) &&
            tag_item != NULL && cJSON_IsString(tag_item)) {
            
            // Copia de forma segura prevenindo estouro de buffers estáticos
            strncpy(received_data.model, model_item->valuestring, sizeof(received_data.model) - 1);
            strncpy(received_data.tag, tag_item->valuestring, sizeof(received_data.tag) - 1);

            ESP_LOGI(TAG, "[STM32_RX]: RFID %s Tag decodificada: %s", 
                     received_data.model, received_data.tag);

            // Envia para a fila global
            if (stm32_data_queue != NULL) {
                if (xQueueSend(stm32_data_queue, &received_data, 0) != pdTRUE) {
                    ESP_LOGW(TAG, "Fila stm32_data_queue cheia. Pacote RFID descartado.");
                }
            }
        } else {
            ESP_LOGW(TAG, "Campos 'model' ou 'tag' ausentes na mensagem do tipo RFID");
        }
    }
    // Lógica para tratamento de dados analógicos de bateria (telemetria)
    else if (strcmp(type_item->valuestring, "batt") == 0) {
        received_data.type = DATA_TYPE_BATTERY;

        cJSON *volt_item = cJSON_GetObjectItem(json, "volt");
        if (volt_item != NULL && cJSON_IsNumber(volt_item)) {
            received_data.battery_v = (float)volt_item->valuedouble;

            ESP_LOGI(TAG, "[STM32_RX]: Telemetria de Bateria: %.2f V", received_data.battery_v);

            // Envia para a fila global
            if (stm32_data_queue != NULL) {
                if (xQueueSend(stm32_data_queue, &received_data, 0) != pdTRUE) {
                    ESP_LOGW(TAG, "Fila stm32_data_queue cheia. Pacote de bateria descartado.");
                }
            }
        } else {
            ESP_LOGW(TAG, "Campo 'volt' ausente ou inválido na mensagem do tipo bateria");
        }
    } 
    else {
        ESP_LOGW(TAG, "Tipo JSON não mapeado no firmware do ESP32: %s", type_item->valuestring);
    }

    // Libera a memória Heap alocada pelo parser do cJSON
    cJSON_Delete(json);
}

/**
 * @brief Task em loop infinito que realiza a leitura de bytes cruificados na UART.
 * @details Implementa um leitor que consome da UART byte a byte de forma não bloqueante.
 *          Concatena esses bytes em um buffer local de caracteres até encontrar o delimitador
 *          de quebra de linha ('\n' ou '\r') que simboliza a finalização de uma string JSON.
 * 
 * @param[in,out] pvParameters Parâmetros padrão do FreeRTOS. Não utilizado neste módulo.
 */
static void stm32_uart_rx_task(void *pvParameters)
{
    // Aloca buffer para acumular caracteres da serial no heap
    uint8_t *data = (uint8_t *) malloc(STM32_UART_BUF_SIZE);
    if (data == NULL) {
        ESP_LOGE(TAG, "Falha crítica de alocação de Heap no início da Task");
        vTaskDelete(NULL);
        return;
    }

    int len = 0;
    ESP_LOGI(TAG, "Escuta serial ativada. Aguardando dados do STM32...");

    while (1) {
        uint8_t byte;
        // Lê um único caractere com timeout curto de 20ms
        int rxBytes = uart_read_bytes(STM32_UART_PORT, &byte, 1, 20 / portTICK_PERIOD_MS);
        
        if (rxBytes > 0) {
            // Delimitador de quebra de linha detectado
            if (byte == '\n' || byte == '\r') {
                if (len > 0) {
                    data[len] = '\0'; // Garante terminação nula para manipulação de string C
                    stm32_uart_parse_json((char *)data);
                    len = 0; // Reseta ponteiro do acumulador
                }
            } else {
                // Acumula os bytes no buffer se não houver estouro de capacidade
                if (len < STM32_UART_BUF_SIZE - 1) {
                    data[len++] = byte;
                } else {
                    ESP_LOGW(TAG, "Estouro de capacidade do buffer serial local. Limpando histórico...");
                    len = 0;
                }
            }
        }
    }

    free(data);
    vTaskDelete(NULL);
}

/* --- Implementação de Funções Públicas --- */

esp_err_t stm32_uart_init(void)
{
    // Estrutura de configuração padrão do periférico UART
    uart_config_t uart_config = {
        .baud_rate = STM32_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    
    // Configura os registradores do periférico selecionado (UART1)
    esp_err_t err = uart_param_config(STM32_UART_PORT, &uart_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao definir os parâmetros da UART. Código: %d", err);
        return err;
    }
    
    // Configura os pinos multiplexados de acordo com o esquema da placa (IO13=RX, IO14=TX)
    err = uart_set_pin(STM32_UART_PORT, STM32_UART_TX_PIN, STM32_UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Erro na atribuição dos pinos físicos da UART. Código: %d", err);
        return err;
    }
    
    // Instala o driver alocando buffer em anel de 2048 bytes para recepção
    err = uart_driver_install(STM32_UART_PORT, STM32_UART_BUF_SIZE * 2, 0, 0, NULL, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Erro na instalação e reserva do driver UART. Código: %d", err);
        return err;
    }
    
    ESP_LOGI(TAG, "Driver serial UART1 ativo com sucesso. Pinos RX:%d TX:%d", STM32_UART_RX_PIN, STM32_UART_TX_PIN);
    return ESP_OK;
}

BaseType_t stm32_uart_rx_task_start(UBaseType_t priority)
{
    // Cria a task FreeRTOS com pilha segura de 4KB para acomodar buffers locais e uso de cJSON
    return xTaskCreate(stm32_uart_rx_task, "stm32_uart_rx_task", 4096, NULL, priority, NULL);
}
