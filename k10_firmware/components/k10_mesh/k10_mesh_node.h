/**
 * @file k10_mesh_node.h
 * @brief Módulo receptor BLE Mesh para a Tela K10
 *
 * Este módulo implementa o nó Mesh que recebe dados do Bastão-ESP
 * (coordenador) e os repassa para a interface gráfica LVGL.
 */
#ifndef K10_MESH_NODE_H
#define K10_MESH_NODE_H

#include "esp_err.h"
#include "gui_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Dados da última tag RFID recebida
 */
typedef struct {
    char model[16];
    char tag[64];
    uint32_t timestamp;
    char animal_name[32];
    float weight;
    char lot[16];
    uint8_t valid;
} k10_last_rfid_t;

/**
 * @brief Dados de bateria recebidos
 */
typedef struct {
    float voltage;
    uint8_t percentage;
    uint8_t critical;
    uint8_t valid;
} k10_battery_status_t;

/**
 * @brief Dados de alerta recebidos
 */
typedef struct {
    char code[32];
    float voltage;
    uint8_t active;
} k10_alert_t;

/**
 * @brief Inicializa o nó BLE Mesh da K10
 *
 * @pre NVS deve estar inicializado
 *
 * @return ESP_OK em caso de sucesso
 */
esp_err_t k10_mesh_node_init(void);

/**
 * @brief Envia dados processados para a UI
 * @details Função callback chamada quando um payload é recebido
 *
 * @param[in] payload JSON descriptografado
 */
void k10_mesh_process_payload(const char *payload);

#ifdef __cplusplus
}
#endif

#endif // K10_MESH_NODE_H