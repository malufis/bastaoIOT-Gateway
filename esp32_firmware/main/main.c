#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "Starting ESP32 Mesh Coordinator");
    
    // TODO: Inicializar rede MESH
    // TODO: Inicializar SIMCom 7663E e PPP
    // TODO: Inicializar UART para receber dados do STM32

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
