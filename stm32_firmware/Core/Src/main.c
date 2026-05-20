/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "alerts.h"
#include "power_mgmt.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */
RFID_Buffer_t buffer_yrm100 = {0};
RFID_Buffer_t buffer_wl134 = {0};
uint8_t byte_yrm100;
uint8_t byte_wl134;

CMD_Buffer_t cmd_buffer = {0};
uint8_t cmd_byte;

uint32_t last_battery_check = 0;
uint32_t last_yrm100_poll = 0;
float battery_voltage = 0.0f;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART4_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART4_UART_Init();
  /* USER CODE BEGIN 2 */
  Alerts_Init();
  Power_Init();

  HAL_GPIO_WritePin(WL134_PWR_PORT, WL134_PWR_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(YRM100_PWR_PORT, YRM100_PWR_PIN, GPIO_PIN_SET);

  HAL_Delay(100);

  HAL_UART_Receive_IT(&huart3, &byte_wl134, 1);
  HAL_UART_Receive_IT(&huart4, &byte_yrm100, 1);
  HAL_UART_Receive_IT(&huart2, &cmd_byte, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
    if (!Power_IsSleeping()) {
        RFID_Process_YRM100();
        RFID_Process_WL134();

        Buzzer_Update();

        Command_Process();

        Power_Update();
    } else {
        HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP, PWR_STOPENTRY_WFI);
        SystemClock_Config();
        Power_Init();

        HAL_UART_Receive_IT(&huart3, &byte_wl134, 1);
        HAL_UART_Receive_IT(&huart4, &byte_yrm100, 1);
        HAL_UART_Receive_IT(&huart2, &cmd_byte, 1);
    }

    if (HAL_GetTick() - last_yrm100_poll > 200) {
        uint8_t cmd_inv[] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};
        HAL_UART_Transmit(&huart4, cmd_inv, sizeof(cmd_inv), 50);
        last_yrm100_poll = HAL_GetTick();
    }

    if (HAL_GetTick() - last_battery_check > 5000) {
        Battery_Read();
        last_battery_check = HAL_GetTick();

        char msg[128];
        sprintf(msg, "{\"type\":\"batt\",\"volt\":%.2f}\n", battery_voltage);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);

        Alerts_CheckBattery(battery_voltage);
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_2;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART4_UART_Init(void)
{

  /* USER CODE BEGIN USART4_Init 0 */

  /* USER CODE END USART4_Init 0 */

  /* USER CODE BEGIN USART4_Init 1 */

  /* USER CODE END USART4_Init 1 */
  huart4.Instance = USART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART4_Init 2 */

  /* USER CODE END USART4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    Power_ActivityDetected();

    if (huart->Instance == USART4) {
        buffer_yrm100.raw_data[buffer_yrm100.head] = byte_yrm100;
        buffer_yrm100.head = (buffer_yrm100.head + 1) % RFID_BUFFER_SIZE;
        HAL_UART_Receive_IT(&huart4, &byte_yrm100, 1);
    } else if (huart->Instance == USART3) {
        buffer_wl134.raw_data[buffer_wl134.head] = byte_wl134;
        buffer_wl134.head = (buffer_wl134.head + 1) % RFID_BUFFER_SIZE;
        HAL_UART_Receive_IT(&huart3, &byte_wl134, 1);
    } else if (huart->Instance == USART2) {
        if (cmd_byte == '\n' || cmd_byte == '\r') {
            if (cmd_buffer.head > 0) {
                cmd_buffer.data[cmd_buffer.head] = '\0';
                Alerts_ProcessCommand((const char*)cmd_buffer.data);
                cmd_buffer.head = 0;
            }
        } else if (cmd_buffer.head < CMD_BUFFER_SIZE - 1) {
            cmd_buffer.data[cmd_buffer.head++] = cmd_byte;
        }
        HAL_UART_Receive_IT(&huart2, &cmd_byte, 1);
    }
}

void Command_Process(void) {
}

void RFID_Process_YRM100(void) {
    while (buffer_yrm100.head != buffer_yrm100.tail) {
        if (buffer_yrm100.raw_data[buffer_yrm100.tail] == 0xBB) {
            uint16_t available = (buffer_yrm100.head + RFID_BUFFER_SIZE - buffer_yrm100.tail) % RFID_BUFFER_SIZE;
            if (available >= 7) {
                uint16_t pl = (buffer_yrm100.raw_data[(buffer_yrm100.tail + 3) % RFID_BUFFER_SIZE] << 8) | 
                             buffer_yrm100.raw_data[(buffer_yrm100.tail + 4) % RFID_BUFFER_SIZE];
                
                if (available >= (7 + pl)) {
                    uint8_t checksum = 0;
                    for (int i = 1; i < 5 + pl; i++) {
                        checksum += buffer_yrm100.raw_data[(buffer_yrm100.tail + i) % RFID_BUFFER_SIZE];
                    }
                    
                    if (checksum == buffer_yrm100.raw_data[(buffer_yrm100.tail + 5 + pl) % RFID_BUFFER_SIZE] &&
                        buffer_yrm100.raw_data[(buffer_yrm100.tail + 6 + pl) % RFID_BUFFER_SIZE] == 0x7E) {
                        
                        if (buffer_yrm100.raw_data[(buffer_yrm100.tail + 2) % RFID_BUFFER_SIZE] == 0x22) {
                            char json[256];
                            char epc_hex[64] = {0};
                            uint16_t epc_len = pl - 5; // Exclude RSSI (1 byte), PC (2 bytes) and CRC (2 bytes)
                            
                            for(int i = 0; i < epc_len; i++) {
                                sprintf(epc_hex + strlen(epc_hex), "%02X", 
                                        buffer_yrm100.raw_data[(buffer_yrm100.tail + 8 + i) % RFID_BUFFER_SIZE]);
                            }
                            sprintf(json, "{\"type\":\"rfid\",\"model\":\"YRM100\",\"tag\":\"%s\"}\n", epc_hex);
                            HAL_UART_Transmit(&huart2, (uint8_t*)json, strlen(json), 100);
                        }
                        
                        buffer_yrm100.tail = (buffer_yrm100.tail + 7 + pl) % RFID_BUFFER_SIZE;
                        continue;
                    }
                } else {
                    return;
                }
            } else {
                return;
            }
        }
        buffer_yrm100.tail = (buffer_yrm100.tail + 1) % RFID_BUFFER_SIZE;
    }
}

void RFID_Process_WL134(void) {
    while (buffer_wl134.head != buffer_wl134.tail) {
        if (buffer_wl134.raw_data[buffer_wl134.tail] == 0x02) {
            uint16_t available = (buffer_wl134.head + RFID_BUFFER_SIZE - buffer_wl134.tail) % RFID_BUFFER_SIZE;
            if (available >= 30) {
                if (buffer_wl134.raw_data[(buffer_wl134.tail + 29) % RFID_BUFFER_SIZE] == 0x03) {
                    uint8_t checksum = 0;
                    for (int i = 1; i <= 26; i++) {
                        checksum ^= buffer_wl134.raw_data[(buffer_wl134.tail + i) % RFID_BUFFER_SIZE];
                    }
                    uint8_t expected_checksum = buffer_wl134.raw_data[(buffer_wl134.tail + 27) % RFID_BUFFER_SIZE];
                    uint8_t expected_inverted = buffer_wl134.raw_data[(buffer_wl134.tail + 28) % RFID_BUFFER_SIZE];
                    
                    if (checksum == expected_checksum && (uint8_t)(~checksum) == expected_inverted) {
                        char card_hex[11] = {0};
                        for (int i = 0; i < 10; i++) {
                            card_hex[i] = buffer_wl134.raw_data[(buffer_wl134.tail + 1 + i) % RFID_BUFFER_SIZE];
                        }
                        char country_hex[5] = {0};
                        for (int i = 0; i < 4; i++) {
                            country_hex[i] = buffer_wl134.raw_data[(buffer_wl134.tail + 11 + i) % RFID_BUFFER_SIZE];
                        }
                        
                        reverse_str(card_hex, 10);
                        reverse_str(country_hex, 4);
                        
                        uint64_t card_dec = hex_to_uint64(card_hex);
                        uint32_t country_dec = (uint32_t)hex_to_uint64(country_hex);
                        
                        char json[128];
                        sprintf(json, "{\"type\":\"rfid\",\"model\":\"WL134\",\"tag\":\"%03lu%012llu\"}\n", 
                                (unsigned long)country_dec, (unsigned long long)card_dec);
                        HAL_UART_Transmit(&huart2, (uint8_t*)json, strlen(json), 100);
                        
                        buffer_wl134.tail = (buffer_wl134.tail + 30) % RFID_BUFFER_SIZE;
                        continue;
                    }
                }
            } else {
                return;
            }
        }
        buffer_wl134.tail = (buffer_wl134.tail + 1) % RFID_BUFFER_SIZE;
    }
}

void reverse_str(char* str, int len) {
    int i = 0;
    int j = len - 1;
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

uint64_t hex_to_uint64(const char* hex_str) {
    uint64_t val = 0;
    while (*hex_str) {
        uint8_t byte = (uint8_t)*hex_str++;
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <= 'f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <= 'F') byte = byte - 'A' + 10;
        else continue;
        val = (val << 4) | byte;
    }
    return val;
}


void Battery_Read(void) {
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        uint32_t val = HAL_ADC_GetValue(&hadc1);
        battery_voltage = ((float)val * 3.3f / 4095.0f) * 11.0f;
    }
    HAL_ADC_Stop(&hadc1);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
