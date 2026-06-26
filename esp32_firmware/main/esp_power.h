/**
 * @file esp_power.h
 * @brief Modulo de gerenciamento de energia (versão simplificada, sem sleep)
 */
#ifndef ESP_POWER_H
#define ESP_POWER_H

#include "esp_err.h"

esp_err_t esp_power_init(void);

#endif // ESP_POWER_H