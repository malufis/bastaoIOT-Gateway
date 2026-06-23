#include "battery_monitor.h"
#include <string.h>
#include <stdio.h>

#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;

static float battery_samples[BATTERY_SAMPLES];
static uint8_t sample_index = 0;
static uint8_t samples_collected = 0;
static float battery_voltage = 0.0f;

void Battery_Init(void)
{
    memset(battery_samples, 0, sizeof(battery_samples));
    sample_index = 0;
    samples_collected = 0;
    battery_voltage = 0.0f;
}

void Battery_Read(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    uint32_t sum_raw = 0;
    uint8_t success_count = 0;

    HAL_ADC_Start(&hadc1);
    for (int i = 0; i < 10; i++) {
        if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
            sum_raw += HAL_ADC_GetValue(&hadc1);
            success_count++;
        }
    }
    HAL_ADC_Stop(&hadc1);

    if (success_count > 0) {
        float avg_raw = (float)sum_raw / (float)success_count;
        float v = (avg_raw * 3.3f / 4095.0f) * BATTERY_DIVIDER_RATIO;

        battery_samples[sample_index] = v;
        sample_index = (sample_index + 1) % BATTERY_SAMPLES;
        if (samples_collected < BATTERY_SAMPLES) {
            samples_collected++;
        }

        float sum = 0.0f;
        uint8_t count = samples_collected > 0 ? samples_collected : 1;
        for (uint8_t i = 0; i < count; i++) {
            sum += battery_samples[i];
        }
        battery_voltage = sum / (float)count;
    }
}

float Battery_GetVoltage(void)
{
    return battery_voltage;
}

uint8_t Battery_IsCritical(void)
{
    return battery_voltage < 8.4f;
}

uint8_t Battery_IsLow(void)
{
    return battery_voltage < 9.6f;
}
