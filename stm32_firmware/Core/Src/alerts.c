/**
 * @file alerts.c
 * @brief Implementação do módulo de alertas e notificação
 *
 * Gerencia alertas de bateria crítica, acionamento do buzzer e
 * parsing de comandos recebidos do ESP32 via UART2.
 */
#include "main.h"
#include "alerts.h"
#include <string.h>
#include <stdio.h>

static AlertState_t alert_state = {0};
static uint8_t battery_critical_flag = 0;

static uint8_t buzzer_state = 0;
static uint32_t buzzer_start_time = 0;

void Alerts_Init(void) {
    memset(&alert_state, 0, sizeof(AlertState_t));
    HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_RESET);
}

void Alerts_CheckBattery(float voltage) {
    alert_state.timestamp = HAL_GetTick();

    if (voltage < BATTERY_CRITICAL_VOLTAGE) {
        if (alert_state.type != ALERT_BATT_CRITICAL) {
            alert_state.type = ALERT_BATT_CRITICAL;
            alert_state.active = 1;
            battery_critical_flag = 1;

            char json[128];
            snprintf(json, sizeof(json),
                "{\"type\":\"alert\",\"code\":\"batt_critical\",\"volt\":%.2f}\n",
                voltage);
            HAL_UART_Transmit(&huart2, (uint8_t*)json, strlen(json), 100);
        }
    } else if (voltage < BATTERY_LOW_VOLTAGE) {
        if (alert_state.type != ALERT_BATT_LOW && alert_state.type != ALERT_BATT_CRITICAL) {
            alert_state.type = ALERT_BATT_LOW;
            alert_state.active = 1;

            char json[128];
            snprintf(json, sizeof(json),
                "{\"type\":\"alert\",\"code\":\"batt_low\",\"volt\":%.2f}\n",
                voltage);
            HAL_UART_Transmit(&huart2, (uint8_t*)json, strlen(json), 100);
        }
    } else {
        if (alert_state.active && (alert_state.type == ALERT_BATT_LOW || alert_state.type == ALERT_BATT_CRITICAL)) {
            alert_state.active = 0;
            alert_state.type = ALERT_NONE;
            battery_critical_flag = 0;
        }
    }
}

void Alerts_ProcessCommand(const char* json) {
    if (json == NULL || strlen(json) == 0) {
        return;
    }

    if (strstr(json, "\"cmd\"") != NULL && strstr(json, "\"buzzer\"") != NULL) {
        if (strstr(json, "\"short\"") != NULL) {
            Buzzer_Play(1);
        } else if (strstr(json, "\"long\"") != NULL) {
            Buzzer_Play(2);
        } else if (strstr(json, "\"double\"") != NULL) {
            Buzzer_Play(3);
        }
    }

    if (strstr(json, "\"cmd\"") != NULL && strstr(json, "\"rfid_on\"") != NULL) {
        HAL_GPIO_WritePin(YRM100_PWR_PORT, YRM100_PWR_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(WL134_PWR_PORT, WL134_PWR_PIN, GPIO_PIN_SET);
    }

    if (strstr(json, "\"cmd\"") != NULL && strstr(json, "\"rfid_off\"") != NULL) {
        HAL_GPIO_WritePin(YRM100_PWR_PORT, YRM100_PWR_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(WL134_PWR_PORT, WL134_PWR_PIN, GPIO_PIN_RESET);
    }

    if (strstr(json, "\"cmd\"") != NULL && strstr(json, "\"yrm_power\"") != NULL) {
        if (strstr(json, "\"value\":1") != NULL || strstr(json, "\"value\":true") != NULL) {
            HAL_GPIO_WritePin(YRM100_PWR_PORT, YRM100_PWR_PIN, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(YRM100_PWR_PORT, YRM100_PWR_PIN, GPIO_PIN_RESET);
        }
    }

    if (strstr(json, "\"cmd\"") != NULL && strstr(json, "\"wl_power\"") != NULL) {
        if (strstr(json, "\"value\":1") != NULL || strstr(json, "\"value\":true") != NULL) {
            HAL_GPIO_WritePin(WL134_PWR_PORT, WL134_PWR_PIN, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(WL134_PWR_PORT, WL134_PWR_PIN, GPIO_PIN_RESET);
        }
    }
}

void Buzzer_Play(uint8_t pattern) {
    buzzer_start_time = HAL_GetTick();
    buzzer_state = pattern;

    switch (pattern) {
        case 1:
            HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_SET);
            break;
        case 2:
            HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_SET);
            break;
        case 3:
            HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_SET);
            break;
        default:
            break;
    }
}

void Buzzer_Stop(void) {
    HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_RESET);
    buzzer_state = 0;
}

uint8_t Alerts_IsBatteryCritical(void) {
    return battery_critical_flag;
}

void Buzzer_Update(void) {
    if (buzzer_state == 0) {
        return;
    }

    uint32_t elapsed = HAL_GetTick() - buzzer_start_time;

    switch (buzzer_state) {
        case 1:
            if (elapsed >= BUZZER_DURATION_SHORT_MS) {
                Buzzer_Stop();
            }
            break;
        case 2:
            if (elapsed >= BUZZER_DURATION_LONG_MS) {
                Buzzer_Stop();
            }
            break;
        case 3:
            if (elapsed >= BUZZER_DURATION_DOUBLE_MS && elapsed < BUZZER_DURATION_DOUBLE_MS + 100) {
                HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_SET);
            } else if (elapsed >= BUZZER_DURATION_DOUBLE_MS + 100 && elapsed < BUZZER_DURATION_DOUBLE_MS * 2) {
                HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_RESET);
            } else if (elapsed >= BUZZER_DURATION_DOUBLE_MS * 2 && elapsed < BUZZER_DURATION_DOUBLE_MS * 2 + BUZZER_DURATION_SHORT_MS) {
                HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_SET);
            } else if (elapsed >= BUZZER_DURATION_DOUBLE_MS * 2 + BUZZER_DURATION_SHORT_MS) {
                Buzzer_Stop();
            }
            break;
        default:
            break;
    }
}