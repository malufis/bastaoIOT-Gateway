/**
 * @file k10_mesh_node.h
 * @brief Módulo receptor BLE Mesh para a Tela K10
 */
#ifndef K10_MESH_NODE_H
#define K10_MESH_NODE_H

#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char model[16];
    char tag[64];
    uint32_t timestamp;
    char animal_name[32];
    float weight;
    char lot[16];
    uint8_t valid;
    float accel_x;
    float accel_y;
    float accel_z;
} k10_last_rfid_t;

typedef struct {
    float voltage;
    uint8_t percentage;
    uint8_t critical;
    uint8_t valid;
} k10_battery_status_t;

typedef struct {
    char code[32];
    float voltage;
    uint8_t active;
} k10_alert_t;

typedef struct {
    double latitude;
    double longitude;
    uint8_t fix;
    float altitude;
    float speed_kmh;
    uint8_t valid;
} k10_gps_data_t;

typedef struct {
    int rssi;
    uint8_t connected;
    char operator_name[32];
    uint8_t valid;
} k10_cell_status_t;

typedef struct {
    uint8_t rfid_connected;
    uint8_t wifi_active;
    uint8_t valid;
} k10_gateway_status_t;

esp_err_t k10_mesh_node_init(void);
void k10_mesh_process_payload(const char *payload);

k10_last_rfid_t* k10_mesh_get_last_rfid(void);
k10_battery_status_t* k10_mesh_get_battery_status(void);
k10_alert_t* k10_mesh_get_alert(void);
k10_gps_data_t* k10_mesh_get_gps_data(void);
k10_cell_status_t* k10_mesh_get_cell_status(void);
k10_gateway_status_t* k10_mesh_get_gateway_status(void);
void k10_mesh_clear_alert(void);

esp_err_t k10_mesh_send_accel(float accel_x, float accel_y, float accel_z, uint8_t movement);

/**
 * @brief Envia status da tela K10 (bateria, eventos UI) para o coordenador
 *        via BLE Mesh.
 * @param battery_voltage Tensao atual da bateria
 * @param battery_percentage Porcentagem estimada (0-100)
 * @param critical 1 se bateria critica
 * @param screen_active 1 se tela ativa/acesa
 * @param ui_event String de evento UI (opcional, pode ser NULL)
 * @return ESP_OK se enviado com sucesso
 */
esp_err_t k10_mesh_send_display_status(float battery_voltage, uint8_t battery_percentage,
                                        uint8_t critical, uint8_t screen_active,
                                        const char *ui_event);

bool k10_mesh_is_ready(void);

#ifdef __cplusplus
}
#endif

#endif
