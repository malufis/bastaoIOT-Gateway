#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "hal_display.h"
#include "gui_manager.h"
#include "hal_sensors.h"
#include "k10_mesh_node.h"

#include "lvgl.h"

static const char *TAG = "K10_MAIN";

/**
 * @brief Thread dedicada à Interface Gráfica LVGL (Fixada no Core 1).
 */
void gui_task(void *pvParameters) {
    ESP_LOGI(TAG, "Iniciando Task LVGL e Sensores");

    hal_display_init();
    hal_sensors_init();

    gui_manager_init();

    uint32_t last_tick = esp_log_timestamp();
    int sensor_timer = 0;

    while (1) {
        uint32_t current_tick = esp_log_timestamp();
        lv_tick_inc(current_tick - last_tick);
        last_tick = current_tick;

        if(sensor_timer++ >= 50) {
            accel_data_t accel;
            battery_data_t bat;
            if(hal_sensors_read_accel(&accel) == ESP_OK && hal_sensors_read_battery(&bat) == ESP_OK) {
                gui_manager_update_sensors(&accel, &bat);
            }

            k10_last_rfid_t *rfid = k10_mesh_get_last_rfid();
            k10_battery_status_t *batt = k10_mesh_get_battery_status();
            k10_alert_t *alert = k10_mesh_get_alert();

            if (rfid && rfid->valid) {
                ESP_LOGI(TAG, "RFID: %s %s", rfid->model, rfid->tag);
                rfid->valid = 0;
            }

            if (batt && batt->valid) {
                ESP_LOGI(TAG, "Batt: %.2fV %d%%", batt->voltage, batt->percentage);
                batt->valid = 0;
            }

            if (alert && alert->active) {
                ESP_LOGW(TAG, "Alert: %s", alert->code);
            }

            sensor_timer = 0;
        }

        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/**
 * @brief Thread dedicada à Rede Mesh (Fixada no Core 0).
 */
void network_task(void *pvParameters) {
    ESP_LOGI(TAG, "Iniciando Task de Rede Mesh");

    esp_err_t ret = k10_mesh_node_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao inicializar nó Mesh: %d", ret);
    }

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief Entry point do firmware K10.
 */
void app_main(void) {
    ESP_LOGI(TAG, "Inicializando K10 Firmware v1.0...");

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    xTaskCreatePinnedToCore(network_task, "network_task", 4096, NULL, 5, NULL, 0);

    xTaskCreatePinnedToCore(gui_task, "gui_task", 1024 * 8, NULL, 5, NULL, 1);

    ESP_LOGI(TAG, "K10 inicializado com sucesso!");
}