/**
 * @file esp_power.c
 * @brief Implementação do gerenciamento de energia do ESP32
 */
#include "esp_power.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "driver/rtc_io.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

static const char *TAG = "ESP_POWER";

static uint64_t last_activity_tick = 0;
static volatile uint8_t sleep_requested = 0;
static esp_power_mode_t current_mode = ESP_POWER_MODE_ACTIVE;
static uint32_t active_timeout_us = ESP_POWER_LIGHT_SLEEP_TIMEOUT_US;
static bool deep_sleep_enabled = false;

esp_err_t esp_power_init(void) {
    last_activity_tick = esp_timer_get_time();
    sleep_requested = 0;
    current_mode = ESP_POWER_MODE_ACTIVE;
    ESP_LOGI(TAG, "Módulo de energia ESP32 inicializado.");
    ESP_LOGI(TAG, "  - Light Sleep timeout: %lu us", ESP_POWER_LIGHT_SLEEP_TIMEOUT_US);
    ESP_LOGI(TAG, "  - Deep Sleep timeout: %lu us", ESP_POWER_DEEP_SLEEP_TIMEOUT_US);
    return ESP_OK;
}

void esp_power_update(void) {
    if (sleep_requested) {
        return;
    }

    uint64_t now_us = esp_timer_get_time();
    uint64_t elapsed_us = now_us - last_activity_tick;

    if (elapsed_us > active_timeout_us) {
        if (deep_sleep_enabled && elapsed_us > ESP_POWER_DEEP_SLEEP_TIMEOUT_US) {
            ESP_LOGI(TAG, "Entrando em deep sleep apos %llu us de inatividade...", elapsed_us);
            esp_power_enter_deep_sleep();
        } else {
            sleep_requested = 1;
            esp_power_enter_light_sleep();
        }
    }
}

void esp_power_enter_light_sleep(void) {
    current_mode = ESP_POWER_MODE_LIGHT_SLEEP;

    esp_sleep_enable_timer_wakeup(active_timeout_us);
    esp_sleep_enable_uart_wakeup(CONFIG_ESP_CONSOLE_UART_NUM);
    esp_sleep_enable_gpio_wakeup(GPIO_NUM_4, ESP_GPIO_WAKEUP_GPIO_HIGH);

    ESP_LOGI(TAG, "Entrando em light sleep...");
    esp_err_t ret = esp_light_sleep_start();

    if (ret == ESP_OK) {
        esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
        ESP_LOGI(TAG, "Acordou do light sleep. Causa: %d", cause);
    } else {
        ESP_LOGE(TAG, "Falha no light sleep: %d", ret);
    }

    sleep_requested = 0;
    current_mode = ESP_POWER_MODE_ACTIVE;
    last_activity_tick = esp_timer_get_time();
}

void esp_power_enter_deep_sleep(void) {
    current_mode = ESP_POWER_MODE_DEEP_SLEEP;
    sleep_requested = 1;

    ESP_LOGI(TAG, "Entrando em deep sleep...");
    ESP_LOGI(TAG, "RTC memory sera preservada para wake-up");

    esp_sleep_enable_timer_wakeup(ESP_POWER_DEEP_SLEEP_TIMEOUT_US);
    esp_sleep_enable_gpio_wakeup(GPIO_NUM_4, ESP_GPIO_WAKEUP_GPIO_HIGH);

    esp_wifi_stop();
    esp_bt_controller_disable(ESP_BT_MODE_IDLE);

    esp_deep_sleep_start();
}

void esp_power_trigger_wake(void) {
    last_activity_tick = esp_timer_get_time();
    if (sleep_requested) {
        sleep_requested = 0;
        ESP_LOGI(TAG, "Wake-up triggered por atividade");
    }
}

uint8_t esp_power_is_active(void) {
    return current_mode == ESP_POWER_MODE_ACTIVE;
}

esp_power_mode_t esp_power_get_mode(void) {
    return current_mode;
}

void esp_power_set_timeout(uint32_t timeout_ms) {
    active_timeout_us = timeout_ms * 1000ULL;
    if (active_timeout_us < 1000000) {
        active_timeout_us = 1000000;
    }
    ESP_LOGI(TAG, "Timeout de inatividade ajustado para %lu ms", timeout_ms);
}

void esp_power_enable_deep_sleep(bool enable) {
    deep_sleep_enabled = enable;
    ESP_LOGI(TAG, "Deep Sleep %s", enable ? "habilitado" : "desabilitado");
}

bool esp_power_is_deep_sleep_enabled(void) {
    return deep_sleep_enabled;
}

uint64_t esp_power_get_idle_time_us(void) {
    return esp_timer_get_time() - last_activity_tick;
}

void esp_power_prepare_for_sleep(void) {
    ESP_LOGI(TAG, "Preparando para dormir...");
}

void esp_power_restore_after_wake(void) {
    ESP_LOGI(TAG, "Restaurando apos wake-up...");

    current_mode = ESP_POWER_MODE_ACTIVE;
    last_activity_tick = esp_timer_get_time();
}