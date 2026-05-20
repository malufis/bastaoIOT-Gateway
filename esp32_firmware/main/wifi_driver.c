/**
 * @file wifi_driver.c
 * @brief Implementação do driver Wi-Fi no modo Station (STA) do ESP32.
 * @details Este módulo implementa a inicialização do stack de Wi-Fi no ESP32,
 *          o registro dos tratadores de eventos de IP e rede, e realiza a
 *          comutação automática da conexão celular PPP para fins de redundância.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#include "wifi_driver.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "simcom_ppp.h"
#include <string.h>

static const char *TAG = "WIFI_DRIVER";
static bool is_connected = false;
static esp_netif_t *wifi_netif = NULL;

/**
 * @brief Tratador de eventos de rede Wi-Fi e IP do ESP-IDF.
 */
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "Wi-Fi iniciado. Conectando...");
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        is_connected = false;
        ESP_LOGW(TAG, "Wi-Fi desconectado ou sinal perdido. Solicitando reconexão e reativando celular...");
        
        // 1. Reativa a conectividade celular via modem SIMCom
        simcom_ppp_set_suspended(false);
        
        // 2. Tenta reconectar ao AP sem fio
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Wi-Fi conectado com sucesso! Endereço IP alocado: " IPSTR, IP2STR(&event->ip_info.ip));
        is_connected = true;
        
        // 1. Suspende a conectividade celular (desconecta PPP) para poupar energia/dados
        simcom_ppp_set_suspended(true);
    }
}

esp_err_t wifi_driver_init(void) {
    ESP_LOGI(TAG, "Inicializando Wi-Fi STA...");

    // Cria a netif padrão para o cliente Wi-Fi Station
    wifi_netif = esp_netif_create_default_wifi_sta();
    if (wifi_netif == NULL) {
        ESP_LOGE(TAG, "Erro ao criar interface de rede padrão Wi-Fi STA.");
        return ESP_FAIL;
    }

    // Inicializa Wi-Fi com a configuração padrão
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_err_t ret = esp_wifi_init(&cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Falha na inicialização do Wi-Fi (%s).", esp_err_to_name(ret));
        return ret;
    }

    // Registra os handlers de eventos
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    // Configura armazenamento temporário em RAM para credenciais
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    
    // Inicia o stack Wi-Fi
    ESP_ERROR_CHECK(esp_wifi_start());

    return ESP_OK;
}

esp_err_t wifi_driver_connect(const char *ssid, const char *password) {
    if (ssid == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    wifi_config_t wifi_config = {0};
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid) - 1);
    if (password != NULL) {
        strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password) - 1);
    }

    ESP_LOGI(TAG, "Configurando Wi-Fi para rede: %s", ssid);
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    
    return esp_wifi_connect();
}

bool wifi_driver_is_connected(void) {
    return is_connected;
}
