#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "hal_sensors.h"

/**
 * @brief Inicializa a engine da interface gráfica LVGL e renderiza a tela Inicial.
 */
void gui_manager_init(void);

/**
 * @brief Atualiza os valores dos sensores na interface gráfica.
 */
void gui_manager_update_sensors(accel_data_t *accel, battery_data_t *bat);

/**
 * @brief Atualiza a UI com dados de tag RFID recebidos via Mesh.
 */
void gui_manager_update_rfid(const char *model, const char *tag,
                             const char *animal_name, float weight, const char *lot);

/**
 * @brief Atualiza a UI com dados de bateria recebidos via Mesh.
 */
void gui_manager_update_battery_mesh(float voltage, uint8_t percentage, uint8_t critical);

/**
 * @brief Atualiza a UI com coordenadas GPS recebidas via Mesh.
 */
void gui_manager_update_gps(double latitude, double longitude, uint8_t fix);

/**
 * @brief Atualiza a UI com status da rede celular recebido via Mesh.
 */
void gui_manager_update_cell_status(int rssi, uint8_t connected, const char *operator_name);
void gui_manager_update_gateway_status(uint8_t rfid_connected, uint8_t wifi_active);

/**
 * @brief Exibe um alerta na interface.
 */
void gui_manager_show_alert(const char *code, float voltage);

/**
 * @brief Limpa o alerta atual.
 */
void gui_manager_clear_alert(void);

/**
 * @brief Exibe tela de leitura RFID (overlay branco com tag por 3s).
 *        Chamada a toda leitura, independente de dedup.
 */
void gui_manager_show_tag_screen(const char *tag);

/**
 * @brief Atualiza a lista de historico de tags (Tab2).
 *
 * Chamado apos cada nova tag registrada no banco de dados.
 */
void gui_manager_refresh_history(void);

#endif
