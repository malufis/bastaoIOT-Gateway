/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "circular_buffer.h"
#include "rfid_parser.h"
#include "battery_monitor.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
#define WL134_PWR_PIN GPIO_PIN_4
#define WL134_PWR_PORT GPIOB
#define YRM100_PWR_PIN GPIO_PIN_5
#define YRM100_PWR_PORT GPIOB
#define BUZZER_PIN GPIO_PIN_6
#define BUZZER_PORT GPIOB

#define CMD_BUFFER_SIZE 256
#define HEARTBEAT_INTERVAL_MS 25000
#define HEARTBEAT_FIRST_MS 5000

typedef enum {
    BUZZER_NONE = 0,
    BUZZER_SHORT = 1,
    BUZZER_LONG = 2,
    BUZZER_DOUBLE = 3
} BuzzerPattern_t;

void Alerts_Init(void);
void Alerts_CheckBattery(float voltage);
void Alerts_ProcessCommand(const char* json);
void Buzzer_Play(BuzzerPattern_t pattern);

void Power_Init(void);

/* LED Status Definition */
#define LED_STATUS_PORT GPIOB
#define LED_STATUS_PIN GPIO_PIN_3

/* Global Peripherals */
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;

/* System and Command Prototypes */
void SystemClock_Config(void);
void Command_Process(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
