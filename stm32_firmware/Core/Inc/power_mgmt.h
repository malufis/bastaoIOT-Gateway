/**
 * @file power_mgmt.h
 * @brief Módulo de gerenciamento de energia e modos de baixo consumo
 */
#ifndef POWER_MGMT_H
#define POWER_MGMT_H

#include "main.h"

#define POWER_SLEEP_TIMEOUT_MS 30000
#define POWER_WAKEUP_INTERVAL_MS 5000

#define POWER_WAKEUP_LPTIM_PERIOD_MS 5000

typedef enum {
    POWER_MODE_ACTIVE = 0,
    POWER_MODE_SLEEP_STOP,
    POWER_MODE_DEEP_SLEEP
} power_mode_t;

void Power_Init(void);
void Power_Update(void);
void Power_Sleep(void);
void Power_EnterDeepSleep(void);
void Power_Wake(void);
uint8_t Power_IsSleeping(void);
void Power_ActivityDetected(void);
power_mode_t Power_GetMode(void);
void Power_SetWakeupInterval(uint32_t interval_ms);

#endif // POWER_MGMT_H