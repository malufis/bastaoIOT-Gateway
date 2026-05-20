/**
 * @file stm32_cmd.c
 * @brief Implementação do módulo de comandos para o STM32
 */
#include "stm32_cmd.h"
#include "stm32_uart.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "STM32_CMD";

esp_err_t stm32_cmd_init(void) {
    ESP_LOGI(TAG, "Módulo de comandos STM32 inicializado.");
    return ESP_OK;
}

esp_err_t stm32_cmd_send_buzzer(stm32_cmd_type_t pattern) {
    char cmd_json[64];

    switch (pattern) {
        case STM32_CMD_BUZZER_SHORT:
            snprintf(cmd_json, sizeof(cmd_json), "{\"cmd\":\"buzzer\",\"type\":\"short\"}\n");
            break;
        case STM32_CMD_BUZZER_LONG:
            snprintf(cmd_json, sizeof(cmd_json), "{\"cmd\":\"buzzer\",\"type\":\"long\"}\n");
            break;
        case STM32_CMD_BUZZER_DOUBLE:
            snprintf(cmd_json, sizeof(cmd_json), "{\"cmd\":\"buzzer\",\"type\":\"double\"}\n");
            break;
        default:
            return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Enviando comando buzzer: %s", cmd_json);
    return stm32_uart_send_string(cmd_json);
}

esp_err_t stm32_cmd_send_rfid_power(uint8_t enable) {
    char cmd_json[64];
    snprintf(cmd_json, sizeof(cmd_json), "{\"cmd\":\"rfid_%s\"}\n", enable ? "on" : "off");

    ESP_LOGI(TAG, "Enviando comando RFID power: %s", cmd_json);
    return stm32_uart_send_string(cmd_json);
}

esp_err_t stm32_cmd_send_yrm_power(uint8_t enable) {
    char cmd_json[64];
    snprintf(cmd_json, sizeof(cmd_json), "{\"cmd\":\"yrm_power\",\"value\":%d}\n", enable ? 1 : 0);

    ESP_LOGI(TAG, "Enviando comando YRM power: %s", cmd_json);
    return stm32_uart_send_string(cmd_json);
}

esp_err_t stm32_cmd_send_wl_power(uint8_t enable) {
    char cmd_json[64];
    snprintf(cmd_json, sizeof(cmd_json), "{\"cmd\":\"wl_power\",\"value\":%d}\n", enable ? 1 : 0);

    ESP_LOGI(TAG, "Enviando comando WL power: %s", cmd_json);
    return stm32_uart_send_string(cmd_json);
}