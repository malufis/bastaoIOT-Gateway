/**
 * @file k10_mesh_node.c
 * @brief Implementação do nó receptor BLE Mesh para a Tela K10
 */
#include "k10_mesh_node.h"
#include <string.h>
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_ble_mesh_mesh_common.h"
#include "esp_ble_mesh_mesh_model.h"
#include "cJSON.h"

static const char *TAG = "K10_MESH";

static k10_last_rfid_t last_rfid = {0};
static k10_battery_status_t battery_status = {0};
static k10_alert_t current_alert = {0};

esp_err_t k10_mesh_node_init(void) {
    ESP_LOGI(TAG, "Inicializando nó BLE Mesh da K10...");

    esp_err_t err = esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "Falha ao liberar memória Classic BT: %d", err);
        return err;
    }

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    err = esp_bt_controller_init(&bt_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao inicializar controlador BT: %d", err);
        return err;
    }

    err = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao habilitar BLE: %d", err);
        return err;
    }

    err = esp_bluedroid_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao inicializar Bluedroid: %d", err);
        return err;
    }

    err = esp_bluedroid_enable();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao habilitar Bluedroid: %d", err);
        return err;
    }

#if defined(CONFIG_BLE_MESH)
    err = esp_ble_mesh_init(NULL);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao inicializar BLE Mesh: %d", err);
        return err;
    }

    ESP_LOGI(TAG, "BLE Mesh nó inicializado com sucesso!");
#else
    ESP_LOGW(TAG, "Suporte a CONFIG_BLE_MESH não ativo.Modo simulado.");
#endif

    memset(&last_rfid, 0, sizeof(last_rfid));
    memset(&battery_status, 0, sizeof(battery_status));
    memset(&current_alert, 0, sizeof(current_alert));

    return ESP_OK;
}

void k10_mesh_process_payload(const char *payload) {
    if (payload == NULL || strlen(payload) == 0) {
        return;
    }

    ESP_LOGI(TAG, "Processando payload: %s", payload);

    cJSON *json = cJSON_Parse(payload);
    if (json == NULL) {
        ESP_LOGE(TAG, "Falha ao parsear JSON");
        return;
    }

    cJSON *type_item = cJSON_GetObjectItem(json, "type");
    if (type_item == NULL || !cJSON_IsString(type_item)) {
        ESP_LOGW(TAG, "Campo 'type' não encontrado");
        cJSON_Delete(json);
        return;
    }

    const char *type = type_item->valuestring;

    if (strcmp(type, "rfid") == 0) {
        cJSON *model = cJSON_GetObjectItem(json, "model");
        cJSON *tag = cJSON_GetObjectItem(json, "tag");
        cJSON *timestamp = cJSON_GetObjectItem(json, "timestamp");
        cJSON *name = cJSON_GetObjectItem(json, "name");
        cJSON *weight = cJSON_GetObjectItem(json, "weight");
        cJSON *lot = cJSON_GetObjectItem(json, "lot");

        if (tag && cJSON_IsString(tag)) {
            strncpy(last_rfid.tag, tag->valuestring, sizeof(last_rfid.tag) - 1);
        }
        if (model && cJSON_IsString(model)) {
            strncpy(last_rfid.model, model->valuestring, sizeof(last_rfid.model) - 1);
        }
        if (timestamp && cJSON_IsNumber(timestamp)) {
            last_rfid.timestamp = (uint32_t)timestamp->valuedouble;
        }
        if (name && cJSON_IsString(name)) {
            strncpy(last_rfid.animal_name, name->valuestring, sizeof(last_rfid.animal_name) - 1);
        }
        if (weight && cJSON_IsNumber(weight)) {
            last_rfid.weight = (float)weight->valuedouble;
        }
        if (lot && cJSON_IsString(lot)) {
            strncpy(last_rfid.lot, lot->valuestring, sizeof(last_rfid.lot) - 1);
        }

        last_rfid.valid = 1;
        ESP_LOGI(TAG, "Tag RFID atualizada: %s (%s)", last_rfid.tag, last_rfid.model);
    }
    else if (strcmp(type, "batt") == 0) {
        cJSON *volt = cJSON_GetObjectItem(json, "volt");
        if (volt && cJSON_IsNumber(volt)) {
            battery_status.voltage = (float)volt->valuedouble;

            if (battery_status.voltage < 8.4f) {
                battery_status.critical = 1;
                battery_status.percentage = 0;
            } else if (battery_status.voltage < 9.6f) {
                battery_status.critical = 0;
                battery_status.percentage = 50;
            } else {
                battery_status.critical = 0;
                battery_status.percentage = 100;
            }

            battery_status.valid = 1;
            ESP_LOGI(TAG, "Bateria atualizada: %.2fV (%d%%)", battery_status.voltage, battery_status.percentage);
        }
    }
    else if (strcmp(type, "alert") == 0) {
        cJSON *code = cJSON_GetObjectItem(json, "code");
        cJSON *volt = cJSON_GetObjectItem(json, "volt");

        if (code && cJSON_IsString(code)) {
            strncpy(current_alert.code, code->valuestring, sizeof(current_alert.code) - 1);
            current_alert.active = 1;
        }
        if (volt && cJSON_IsNumber(volt)) {
            current_alert.voltage = (float)volt->valuedouble;
        }

        ESP_LOGW(TAG, "Alerta recebido: %s (%.2fV)", current_alert.code, current_alert.voltage);
    }

    cJSON_Delete(json);
}

k10_last_rfid_t* k10_mesh_get_last_rfid(void) {
    return &last_rfid;
}

k10_battery_status_t* k10_mesh_get_battery_status(void) {
    return &battery_status;
}

k10_alert_t* k10_mesh_get_alert(void) {
    return &current_alert;
}

void k10_mesh_clear_alert(void) {
    memset(&current_alert, 0, sizeof(current_alert));
}