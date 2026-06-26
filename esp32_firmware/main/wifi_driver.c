#include "wifi_driver.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mac.h"
#include <string.h>

static const char *TAG = "WIFI_DRIVER";
static bool is_sta_connected = false;
static esp_netif_t *sta_netif = NULL;
static esp_netif_t *ap_netif = NULL;
static char ap_ip_str[16] = {0};

#define AP_SSID_PREFIX "Bastao-"
#define AP_PASSWORD    "bastao123"
#define AP_MAX_CONN    3
#define AP_CHANNEL     6

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "Wi-Fi STA iniciado. Conectando...");
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        is_sta_connected = false;
        ESP_LOGW(TAG, "Wi-Fi STA desconectado ou sinal perdido.");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Wi-Fi STA conectado! IP: " IPSTR, IP2STR(&event->ip_info.ip));
        is_sta_connected = true;
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* ev = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "Cliente conectou ao AP (MAC: " MACSTR ")", MAC2STR(ev->mac));
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* ev = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "Cliente desconectou do AP (MAC: " MACSTR ")", MAC2STR(ev->mac));
    }
}

static void start_softap_after_init(void) {
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    char ap_ssid[32];
    snprintf(ap_ssid, sizeof(ap_ssid), "%s%02X%02X%02X",
             AP_SSID_PREFIX, mac[3], mac[4], mac[5]);

    esp_netif_ip_info_t ip_info = {
        .ip = { .addr = ESP_IP4TOADDR(192, 168, 4, 1) },
        .gw = { .addr = ESP_IP4TOADDR(192, 168, 4, 1) },
        .netmask = { .addr = ESP_IP4TOADDR(255, 255, 255, 0) },
    };
    esp_netif_set_ip_info(ap_netif, &ip_info);
    snprintf(ap_ip_str, sizeof(ap_ip_str), IPSTR, IP2STR(&ip_info.ip));

    wifi_config_t ap_config = {
        .ap = {
            .ssid_len = 0,
            .channel = AP_CHANNEL,
            .max_connection = AP_MAX_CONN,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .password = AP_PASSWORD,
            .pmf_cfg = { .required = false },
        },
    };
    strncpy((char *)ap_config.ap.ssid, ap_ssid, sizeof(ap_config.ap.ssid) - 1);

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
    ESP_LOGI(TAG, "SoftAP iniciado: SSID=\"%s\" senha=\"%s\" IP=%s canal=%d max_conn=%d)",
             ap_ssid, AP_PASSWORD, ap_ip_str, AP_CHANNEL, AP_MAX_CONN);
}

esp_err_t wifi_driver_init(void) {
    ESP_LOGI(TAG, "Inicializando Wi-Fi AP+STA...");

    // Cria ambas as interfaces ANTES de esp_wifi_init()
    sta_netif = esp_netif_create_default_wifi_sta();
    if (sta_netif == NULL) {
        ESP_LOGE(TAG, "Falha ao criar interface STA.");
        return ESP_FAIL;
    }
    ap_netif = esp_netif_create_default_wifi_ap();
    if (ap_netif == NULL) {
        ESP_LOGE(TAG, "Falha ao criar interface AP.");
        return ESP_FAIL;
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_err_t ret = esp_wifi_init(&cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao init Wi-Fi: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    // Configura AP ANTES de esp_wifi_start() (ESP-IDF exige config antes de start)
    start_softap_after_init();

    ESP_ERROR_CHECK(esp_wifi_start());

    return ESP_OK;
}

esp_err_t wifi_driver_connect(const char *ssid, const char *password) {
    if (ssid == NULL) return ESP_ERR_INVALID_ARG;

    wifi_config_t wifi_config = {0};
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid) - 1);
    if (password != NULL) {
        strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password) - 1);
    }

    ESP_LOGI(TAG, "Conectando STA a rede: %s", ssid);
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    return esp_wifi_connect();
}

bool wifi_driver_is_connected(void) {
    return is_sta_connected;
}

esp_err_t wifi_driver_disconnect(void) {
    ESP_LOGI(TAG, "Desconectando Wi-Fi STA...");
    return esp_wifi_disconnect();
}

const char *wifi_driver_get_ap_ip(void) {
    return ap_ip_str;
}
