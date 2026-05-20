/**
 * @file cmd_parser.c
 * @brief Implementação do parser de comandos MQTT
 */
#include "cmd_parser.h"
#include "stm32_cmd.h"
#include "esp_log.h"
#include "cJSON.h"
#include <string.h>

static const char *TAG = "CMD_PARSER";

esp_err_t cmd_parser_init(void) {
    ESP_LOGI(TAG, "Parser de comandos MQTT inicializado.");
    return ESP_OK;
}

void cmd_parser_process_message(const char *topic, const char *payload) {
    if (payload == NULL) {
        return;
    }

    ESP_LOGI(TAG, "Comando MQTT recebido: %s", payload);

    cJSON *json = cJSON_Parse(payload);
    if (json == NULL) {
        ESP_LOGE(TAG, "Falha ao parsear JSON do comando");
        return;
    }

    cJSON *cmd_item = cJSON_GetObjectItem(json, "cmd");
    if (cmd_item == NULL || !cJSON_IsString(cmd_item)) {
        ESP_LOGW(TAG, "Campo 'cmd' não encontrado no JSON");
        cJSON_Delete(json);
        return;
    }

    const char *cmd = cmd_item->valuestring;

    if (strcmp(cmd, "buzzer") == 0) {
        cJSON *type_item = cJSON_GetObjectItem(json, "type");
        if (type_item != NULL && cJSON_IsString(type_item)) {
            if (strcmp(type_item->valuestring, "short") == 0) {
                stm32_cmd_send_buzzer(STM32_CMD_BUZZER_SHORT);
            } else if (strcmp(type_item->valuestring, "long") == 0) {
                stm32_cmd_send_buzzer(STM32_CMD_BUZZER_LONG);
            } else if (strcmp(type_item->valuestring, "double") == 0) {
                stm32_cmd_send_buzzer(STM32_CMD_BUZZER_DOUBLE);
            }
        }
    }
    else if (strcmp(cmd, "rfid_on") == 0) {
        stm32_cmd_send_rfid_power(1);
    }
    else if (strcmp(cmd, "rfid_off") == 0) {
        stm32_cmd_send_rfid_power(0);
    }
    else if (strcmp(cmd, "yrm_power") == 0) {
        cJSON *value_item = cJSON_GetObjectItem(json, "value");
        if (value_item != NULL) {
            uint8_t enable = cJSON_IsTrue(value_item) || value_item->valueint == 1;
            stm32_cmd_send_yrm_power(enable);
        }
    }
    else if (strcmp(cmd, "wl_power") == 0) {
        cJSON *value_item = cJSON_GetObjectItem(json, "value");
        if (value_item != NULL) {
            uint8_t enable = cJSON_IsTrue(value_item) || value_item->valueint == 1;
            stm32_cmd_send_wl_power(enable);
        }
    }
    else if (strcmp(cmd, "ota") == 0) {
        cJSON *url_item = cJSON_GetObjectItem(json, "url");
        if (url_item != NULL && cJSON_IsString(url_item)) {
            ESP_LOGI(TAG, "Comando OTA recebido, URL: %s", url_item->valuestring);
            // O OTA manager deve tratar este comando
        }
    }
    else {
        ESP_LOGW(TAG, "Comando desconhecido: %s", cmd);
    }

    cJSON_Delete(json);
}