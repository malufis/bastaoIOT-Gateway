/**
 * @file power_mgmt.c
 * @brief Implementacao do gerenciamento de energia (versao simplificada, sem sleep)
 */
#include "main.h"
#include "power_mgmt.h"

void Power_Init(void) {
    HAL_GPIO_WritePin(WL134_PWR_PORT, WL134_PWR_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(YRM100_PWR_PORT, YRM100_PWR_PIN, GPIO_PIN_SET);
}
