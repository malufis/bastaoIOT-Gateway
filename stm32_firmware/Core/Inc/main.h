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

#define RFID_BUFFER_SIZE 128
#define CMD_BUFFER_SIZE 256
#define BATTERY_CRITICAL_THRESHOLD 15.0f

typedef struct {
    uint8_t raw_data[RFID_BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
    uint8_t flag_new_data;
} RFID_Buffer_t;

typedef struct {
    uint8_t data[CMD_BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
} CMD_Buffer_t;

typedef enum {
    BUZZER_NONE = 0,
    BUZZER_SHORT = 1,
    BUZZER_LONG = 2,
    BUZZER_DOUBLE = 3
} BuzzerPattern_t;

void RFID_Process_YRM100(void);
void RFID_Process_WL134(void);
void Battery_Read(void);
void reverse_str(char* str, int len);
uint64_t hex_to_uint64(const char* hex_str);

void Alerts_Init(void);
void Alerts_CheckBattery(float voltage);
void Alerts_ProcessCommand(const char* json);
void Buzzer_Play(BuzzerPattern_t pattern);

void Power_Init(void);
void Power_Update(void);
void Power_Sleep(void);
void Power_Wake(void);
uint8_t Power_IsSleeping(void);
void Power_ActivityDetected(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
