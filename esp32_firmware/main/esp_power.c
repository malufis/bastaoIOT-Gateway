/**
 * @file esp_power.c
 * @brief Implementacao do gerenciamento de energia (versão simplificada, sem sleep)
 */
#include "esp_power.h"
#include "esp_log.h"

static const char *TAG = "ESP_POWER";

esp_err_t esp_power_init(void) {
    ESP_LOGI(TAG, "Modulo de energia ESP32 inicializado (sleep desabilitado).");
    return ESP_OK;
}