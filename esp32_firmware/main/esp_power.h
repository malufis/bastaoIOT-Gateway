/**
 * @file esp_power.h
 * @brief Módulo de gerenciamento de energia para o ESP32
 *
 * Controla os modos de baixo consumo (light sleep, deep sleep) e
 * wake-up sources baseados em atividade RFID ou temporizador.
 */
#ifndef ESP_POWER_H
#define ESP_POWER_H

#include "esp_err.h"

#define ESP_POWER_TIMEOUT_MS 60000
#define ESP_POWER_LIGHT_SLEEP_TIMEOUT_US 60000000
#define ESP_POWER_DEEP_SLEEP_TIMEOUT_US 300000000

typedef enum {
    ESP_POWER_MODE_ACTIVE = 0,
    ESP_POWER_MODE_LIGHT_SLEEP,
    ESP_POWER_MODE_DEEP_SLEEP
} esp_power_mode_t;

esp_err_t esp_power_init(void);
void esp_power_update(void);
void esp_power_trigger_wake(void);
uint8_t esp_power_is_active(void);
esp_power_mode_t esp_power_get_mode(void);
void esp_power_enter_light_sleep(void);
void esp_power_enter_deep_sleep(void);
void esp_power_set_timeout(uint32_t timeout_ms);

#endif // ESP_POWER_H