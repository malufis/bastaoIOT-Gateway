#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <stdint.h>

#define BATTERY_SAMPLES 8

void Battery_Init(void);
void Battery_Read(void);
float Battery_GetVoltage(void);
uint8_t Battery_IsCritical(void);
uint8_t Battery_IsLow(void);

#endif
