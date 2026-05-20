#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "hal_sensors.h"

void gui_manager_init(void);
void gui_manager_update_sensors(accel_data_t *accel, battery_data_t *bat);

/**
 * @brief Atualiza a UI com dados de tag RFID recebidos via Mesh
 */
void gui_manager_update_rfid(const char *model, const char *tag,
                             const char *animal_name, float weight, const char *lot);

/**
 * @brief Atualiza a UI com dados de bateria recebidos via Mesh
 */
void gui_manager_update_battery_mesh(float voltage, uint8_t percentage, uint8_t critical);

/**
 * @brief Exibe um alerta na interface
 */
void gui_manager_show_alert(const char *code, float voltage);

/**
 * @brief Limpa o alerta atual
 */
void gui_manager_clear_alert(void);

#endif