/**
 * @file alerts.h
 * @brief Módulo de alertas e notificação do sistema Bastão-ESP
 *
 * Gerencia alertas de bateria crítica e acionamento do buzzer.
 */
#ifndef ALERTS_H
#define ALERTS_H

#include "main.h"

#define BUZZER_GPIO_PORT GPIOB
#define BUZZER_GPIO_PIN  GPIO_PIN_6

#define BATTERY_CRITICAL_VOLTAGE 8.4f
#define BATTERY_LOW_VOLTAGE 9.6f

#define BUZZER_DURATION_SHORT_MS 100
#define BUZZER_DURATION_LONG_MS 500
#define BUZZER_DURATION_DOUBLE_MS 150

typedef enum {
    ALERT_NONE = 0,
    ALERT_BATT_LOW = 1,
    ALERT_BATT_CRITICAL = 2,
    ALERT_CACHE_FULL = 3,
    ALERT_RFID_ERROR = 4
} AlertType_t;

typedef struct {
    AlertType_t type;
    uint8_t active;
    uint32_t timestamp;
} AlertState_t;

void Alerts_Init(void);
void Alerts_CheckBattery(float voltage);
void Alerts_ProcessCommand(const char* json);
void Buzzer_Play(uint8_t pattern);
void Buzzer_Stop(void);
uint8_t Alerts_IsBatteryCritical(void);

#endif // ALERTS_H