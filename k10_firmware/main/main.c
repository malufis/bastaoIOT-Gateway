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
    
    // 1. Inicializa driver LCD, Touch e Sensores
    hal_display_init();
    hal_sensors_init();

    // 2. Inicializa LVGL e Gerenciador de UI
    gui_manager_init();

    uint32_t last_tick = esp_log_timestamp();
    int sensor_timer = 0;

    while (1) {
        uint32_t current_tick = esp_log_timestamp();
        lv_tick_inc(current_tick - last_tick);
        last_tick = current_tick;

        // Atualiza sensores locais e dados Mesh a cada 50ms
        if(sensor_timer++ >= 5) { 
            accel_data_t accel;
            battery_data_t bat;
            if(hal_sensors_read_accel(&accel) == ESP_OK && hal_sensors_read_battery(&bat) == ESP_OK) {
                gui_manager_update_sensors(&accel, &bat);

                // Envia acelerometro via BLE Mesh para o coordenador
                if (k10_mesh_is_ready()) {
                    // Detecta movimento: > 10% de 1G em qualquer eixo
                    static float last_x = 0, last_y = 0, last_z = 0;
                    static uint32_t last_send_time = 0;
                    float dx = accel.x - last_x;
                    float dy = accel.y - last_y;
                    float dz = accel.z - last_z;
                    uint8_t movement = (dx*dx + dy*dy + dz*dz > 0.1f) ? 1 : 0;

                    uint32_t now = esp_log_timestamp();
                    // Envia apenas se houver movimento detectado ou se ja se passaram 10s (heartbeat)
                    if (movement || (now - last_send_time >= 10000)) {
                        k10_mesh_send_accel(accel.x, accel.y, accel.z, movement);
                        last_send_time = now;
                    }
                    last_x = accel.x; last_y = accel.y; last_z = accel.z;
                }
            }

            // Poll dados recebidos via Mesh
            k10_last_rfid_t *rfid = k10_mesh_get_last_rfid();
            if (rfid && rfid->valid) {
                hal_display_reset_inactivity();
                ESP_LOGI(TAG, "RFID via Mesh: %s %s", rfid->model, rfid->tag);
                gui_manager_update_rfid(rfid->model, rfid->tag,
                                        rfid->animal_name, rfid->weight, rfid->lot);

                // Envia imediatamente bateria e acelerometro da K10 de volta para o coordenador
                accel_data_t immediate_accel;
                battery_data_t immediate_bat;
                bool read_accel_ok = (hal_sensors_read_accel(&immediate_accel) == ESP_OK);
                bool read_bat_ok = (hal_sensors_read_battery(&immediate_bat) == ESP_OK);

                if (read_accel_ok) {
                    gui_manager_update_sensors(&immediate_accel, read_bat_ok ? &immediate_bat : NULL);

                    static float last_x = 0, last_y = 0, last_z = 0;
                    float dx = immediate_accel.x - last_x;
                    float dy = immediate_accel.y - last_y;
                    float dz = immediate_accel.z - last_z;
                    uint8_t movement = (dx*dx + dy*dy + dz*dz > 0.1f) ? 1 : 0;
                    k10_mesh_send_accel(immediate_accel.x, immediate_accel.y, immediate_accel.z, movement);
                    last_x = immediate_accel.x; last_y = immediate_accel.y; last_z = immediate_accel.z;
                } else if (read_bat_ok) {
                    gui_manager_update_sensors(NULL, &immediate_bat);
                }

                if (read_bat_ok) {
                    k10_mesh_send_display_status(immediate_bat.voltage, (uint8_t)immediate_bat.percentage,
                                                  (immediate_bat.percentage < 10) ? 1 : 0,
                                                  1, NULL);
                }

                rfid->valid = 0;
            }
            k10_battery_status_t *mesh_bat = k10_mesh_get_battery_status();
            if (mesh_bat && mesh_bat->valid) {
                ESP_LOGI(TAG, "Bat via Mesh: %.2fV %d%%", mesh_bat->voltage, mesh_bat->percentage);
                gui_manager_update_battery_mesh(mesh_bat->voltage, mesh_bat->percentage, 0);
                mesh_bat->valid = 0;
            }
            k10_gps_data_t *gps = k10_mesh_get_gps_data();
            if (gps && gps->valid) {
                ESP_LOGI(TAG, "GPS via Mesh: %.6f, %.6f fix=%d", gps->latitude, gps->longitude, gps->fix);
                gui_manager_update_gps(gps->latitude, gps->longitude, gps->fix);
                gps->valid = 0;
            }
            k10_cell_status_t *cell = k10_mesh_get_cell_status();
            if (cell && cell->valid) {
                ESP_LOGI(TAG, "Cell via Mesh: rssi=%d conn=%d op=%s", cell->rssi, cell->connected, cell->operator_name);
                gui_manager_update_cell_status(cell->rssi, cell->connected, cell->operator_name);
                cell->valid = 0;
            }
            k10_gateway_status_t *gw = k10_mesh_get_gateway_status();
            if (gw && gw->valid) {
                gui_manager_update_gateway_status(gw->rfid_connected, gw->wifi_active);
                gw->valid = 0;
            }

            sensor_timer = 0;
        }

        // Envia status da tela (bateria) para o gateway a cada ~30s
        {
            static uint32_t display_status_tick = 0;
            display_status_tick++;
            if (display_status_tick >= 3000) {  /* 3000 * 10ms = 30s */
                display_status_tick = 0;
                if (k10_mesh_is_ready()) {
                    battery_data_t bat;
                    if (hal_sensors_read_battery(&bat) == ESP_OK) {
                        k10_mesh_send_display_status(bat.voltage, (uint8_t)bat.percentage,
                                                      (bat.percentage < 10) ? 1 : 0,
                                                      1, NULL);
                    }
                }
            }
        }

        lv_timer_handler();
        hal_display_check_timeout();
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
        ESP_LOGE(TAG, "Falha ao inicializar no Mesh: %d", ret);
    }

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief Entry point do firmware K10.
 */
void app_main(void) {
    ESP_LOGI(TAG, "Inicializando Bastão Unihiker K10...");

    // Inicializa NVS (Non-Volatile Storage) para configs de rede
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Task de Rede (Mesh/Gateway) no Core 0
    // Prioridade 1 (ociosa) — o stack BLE Mesh roda nos callbacks internos
    xTaskCreatePinnedToCore(network_task, "network_task", 8192, NULL, 1, NULL, 0);

    // Task da Interface Gráfica no Core 1
    xTaskCreatePinnedToCore(gui_task, "gui_task", 1024 * 8, NULL, 5, NULL, 1);

    // Aguarda tasks iniciarem e loga uso de stack
    vTaskDelay(pdMS_TO_TICKS(200));
    {
        static const char * const task_names[] = {
            "network_task", "gui_task"
        };
        ESP_LOGI(TAG, "=== Monitoramento de Stack FreeRTOS ===");
        for (int i = 0; i < sizeof(task_names) / sizeof(task_names[0]); i++) {
            TaskHandle_t h = xTaskGetHandle(task_names[i]);
            if (h != NULL) {
                UBaseType_t free = uxTaskGetStackHighWaterMark(h);
                ESP_LOGI(TAG, "  %-22s  %4u bytes livres", task_names[i], free);
            } else {
                ESP_LOGW(TAG, "  %-22s  (handle nao encontrado)", task_names[i]);
            }
        }
        ESP_LOGI(TAG, "=========================================");
    }
}
