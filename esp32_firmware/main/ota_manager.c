/**
 * @file ota_manager.c
 * @brief Implementacao do gerenciador de atualizacoes de firmware remotas (OTA).
 * @details Este modulo implementa a logica de validacao pos-boot (cancelamento de rollback),
 *          verificacao de interfaces de rede ativas (Wi-Fi ou 4G/PPP) e aciona
 *          a rotina do ESP-IDF esp_https_ota para baixar a imagem.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#include "ota_manager.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_crt_bundle.h"
#include "esp_netif.h"
#include "simcom_driver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "OTA_MANAGER";
static char ota_url[256];

/**
 * @brief Task FreeRTOS encarregada de executar o download e a gravacao do OTA.
 */
static void ota_task(void *pvParameters) {
    ESP_LOGI(TAG, "Iniciando tarefa de atualizacao OTA...");

    // 1. Identifica qual interface de rede esta ativa
    esp_netif_t *wifi_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    bool wifi_connected = false;
    if (wifi_netif != NULL) {
        wifi_connected = esp_netif_is_netif_up(wifi_netif);
    }

    if (wifi_connected) {
        ESP_LOGI(TAG, "Interface Wi-Fi ativa. Download OTA iniciado via rede local.");
    } else {
        ESP_LOGE(TAG, "Sem rota TCP/IP ativa. Atualizacao OTA restrita ao Wi-Fi (PPP desativado). Abortando OTA.");
        vTaskDelete(NULL);
        return;
    }

    ESP_LOGI(TAG, "Conectando ao servidor HTTPS para baixar binario: %s", ota_url);

    esp_http_client_config_t http_config = {
        .url = ota_url,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .keep_alive_enable = true,
    };

    esp_https_ota_config_t ota_config = {
        .http_config = &http_config,
    };

    esp_err_t ret = esp_https_ota(&ota_config);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Atualizacao OTA gravada com sucesso! Reiniciando o bastao em 3 segundos...");
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        esp_restart();
    } else {
        ESP_LOGE(TAG, "Falha critica no download/gravacao da imagem OTA: %s", esp_err_to_name(ret));
    }

    vTaskDelete(NULL);
}

esp_err_t ota_manager_init(void) {
    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_ota_img_states_t ota_state;

    if (esp_ota_get_state_partition(running, &ota_state) == ESP_OK) {
        if (ota_state == ESP_OTA_IMG_PENDING_VERIFY) {
            ESP_LOGI(TAG, "Novo firmware detectado em estado PENDING_VERIFY. Validando inicializacao...");
            esp_err_t err = esp_ota_mark_app_valid_cancel_rollback();
            if (err == ESP_OK) {
                ESP_LOGI(TAG, "Firmware validado com sucesso! Rollback de seguranca cancelado.");
            } else {
                ESP_LOGE(TAG, "Falha ao cancelar o rollback de seguranca: %s", esp_err_to_name(err));
                return err;
            }
        } else {
            ESP_LOGI(TAG, "Particao de execucao atual esta estavel (Estado: %d).", ota_state);
        }
    }
    return ESP_OK;
}

esp_err_t ota_manager_start(const char *bin_url) {
    if (bin_url == NULL || strlen(bin_url) >= sizeof(ota_url)) {
        return ESP_ERR_INVALID_ARG;
    }
    strncpy(ota_url, bin_url, sizeof(ota_url) - 1);

    BaseType_t ret = xTaskCreatePinnedToCore(ota_task, "ota_task", 8192, NULL, 4, NULL, 1);
    return (ret == pdPASS) ? ESP_OK : ESP_FAIL;
}
