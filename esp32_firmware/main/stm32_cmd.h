/**
 * @file stm32_cmd.h
 * @brief Módulo de envio de comandos para o STM32 via UART2
 *
 * Responsável por enviar comandos de controle (buzzer, power, etc)
 * para o microcontrolador STM32G070CBTx.
 */
#ifndef STM32_CMD_H
#define STM32_CMD_H

#include "esp_err.h"

typedef enum {
    STM32_CMD_BUZZER_SHORT = 0,
    STM32_CMD_BUZZER_LONG,
    STM32_CMD_BUZZER_DOUBLE,
    STM32_CMD_RFID_ON,
    STM32_CMD_RFID_OFF,
    STM32_CMD_YRM_POWER,
    STM32_CMD_WL_POWER,
} stm32_cmd_type_t;

esp_err_t stm32_cmd_init(void);
esp_err_t stm32_cmd_send_buzzer(stm32_cmd_type_t pattern);
esp_err_t stm32_cmd_send_rfid_power(uint8_t enable);
esp_err_t stm32_cmd_send_yrm_power(uint8_t enable);
esp_err_t stm32_cmd_send_wl_power(uint8_t enable);

#endif // STM32_CMD_H