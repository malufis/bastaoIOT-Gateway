/**
 * @file power_mgmt.c
 * @brief Implementação do gerenciamento de energia e modos de baixo consumo
 */
#include "main.h"
#include "power_mgmt.h"
#include "alerts.h"

static uint32_t last_activity_time = 0;
static volatile uint8_t sleep_mode_active = 0;
static power_mode_t current_power_mode = POWER_MODE_ACTIVE;
static uint32_t wakeup_interval_ms = POWER_WAKEUP_INTERVAL_MS;

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;

void Power_Init(void) {
    last_activity_time = HAL_GetTick();
    sleep_mode_active = 0;
    current_power_mode = POWER_MODE_ACTIVE;
    HAL_GPIO_WritePin(WL134_PWR_PORT, WL134_PWR_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(YRM100_PWR_PORT, YRM100_PWR_PIN, GPIO_PIN_SET);
}

void Power_Update(void) {
    if (sleep_mode_active) {
        return;
    }

    uint32_t now = HAL_GetTick();
    if (now - last_activity_time > POWER_SLEEP_TIMEOUT_MS) {
        Power_Sleep();
    }
}

void Power_Sleep(void) {
    sleep_mode_active = 1;
    current_power_mode = POWER_MODE_SLEEP_STOP;

    HAL_GPIO_WritePin(WL134_PWR_PORT, WL134_PWR_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(YRM100_PWR_PORT, YRM100_PWR_PIN, GPIO_PIN_RESET);

    HAL_SuspendTick();
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP, PWR_STOPENTRY_WFI);
    SystemClock_Config();

    HAL_ResumeTick();

    HAL_Delay(100);
    HAL_GPIO_WritePin(WL134_PWR_PORT, WL134_PWR_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(YRM100_PWR_PORT, YRM100_PWR_PIN, GPIO_PIN_SET);
    HAL_Delay(50);

    sleep_mode_active = 0;
    current_power_mode = POWER_MODE_ACTIVE;
    last_activity_time = HAL_GetTick();
}

void Power_EnterDeepSleep(void) {
    sleep_mode_active = 1;
    current_power_mode = POWER_MODE_DEEP_SLEEP;

    HAL_GPIO_WritePin(WL134_PWR_PORT, WL134_PWR_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(YRM100_PWR_PORT, YRM100_PWR_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_STATUS_PORT, LED_STATUS_PIN, GPIO_PIN_RESET);

    __HAL_RCC_WAKEUPSTOP_CLOCK_SOURCE_CONFIG(RCC_STOP_WAKEUPCLOCK_LSI);

    HAL_PWREx_EnableLowPowerRunMode();

    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP, PWR_STOPENTRY_WFI);
}

void Power_Wake(void) {
    last_activity_time = HAL_GetTick();
    if (sleep_mode_active) {
        sleep_mode_active = 0;
        current_power_mode = POWER_MODE_ACTIVE;
    }
}

uint8_t Power_IsSleeping(void) {
    return sleep_mode_active;
}

void Power_ActivityDetected(void) {
    last_activity_time = HAL_GetTick();
    if (sleep_mode_active) {
        sleep_mode_active = 0;
        current_power_mode = POWER_MODE_ACTIVE;
    }
}

power_mode_t Power_GetMode(void) {
    return current_power_mode;
}

void Power_SetWakeupInterval(uint32_t interval_ms) {
    wakeup_interval_ms = (interval_ms < 1000) ? 1000 : interval_ms;
}

uint32_t Power_GetWakeupInterval(void) {
    return wakeup_interval_ms;
}

void Power_EnableDeepSleep(uint8_t enable) {
    if (enable) {
        __HAL_RCC_WAKEUPSTOP_CLOCK_SOURCE_CONFIG(RCC_STOP_WAKEUPCLOCK_LSI);
        HAL_PWREx_EnablePullUpPullUpConfig(PWR_PULLUP_PULLUP_GPIO0 | PWR_PULLUP_PULLUP_GPIO1);
    }
}

void Power_ReInitPeripherals(void) {
    SystemClock_Config();

    HAL_Delay(100);

    HAL_GPIO_WritePin(WL134_PWR_PORT, WL134_PWR_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(YRM100_PWR_PORT, YRM100_PWR_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_STATUS_PORT, LED_STATUS_PIN, GPIO_PIN_SET);

    HAL_Delay(50);

    sleep_mode_active = 0;
    current_power_mode = POWER_MODE_ACTIVE;
    last_activity_time = HAL_GetTick();
}