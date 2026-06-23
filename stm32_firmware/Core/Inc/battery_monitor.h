#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <stdint.h>

#define BATTERY_SAMPLES 10

/** @brief Relacao do divisor resistivo: (R1 + R2) / R2.
 *  Para R1=100k, R2=10k: (100+10)/10 = 11.0
 *  Para calibrar: meca a tensao real com multimetro (Vreal) e veja o valor
 *  exibido (Vexibido). Novo ratio = 11.0 * Vreal / Vexibido.
 */
#define BATTERY_DIVIDER_RATIO 10.63f

void Battery_Init(void);
void Battery_Read(void);
float Battery_GetVoltage(void);
uint8_t Battery_IsCritical(void);
uint8_t Battery_IsLow(void);

#endif
