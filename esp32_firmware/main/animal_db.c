/**
 * @file animal_db.c
 * @brief Implementação do banco de dados local de animais do Bastão-ESP.
 * @details Este módulo é responsável por carregar e realizar buscas na
 *          base de dados de negócios (Fazenda, Lote, Animal) persistida na NVS.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#include "animal_db.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "cJSON.h"
#include <string.h>
#include <stdlib.h>

static const char *TAG = "ANIMAL_DB";

esp_err_t animal_db_init(void) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open("bastao_biz", NVS_READONLY, &handle);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Módulo de negócios não inicializado na NVS (bastao_biz).");
        return ESP_OK; // Não é um erro impeditivo no boot
    }

    size_t required_size = 0;
    err = nvs_get_str(handle, "biz_json", NULL, &required_size);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Base de dados local de animais encontrada. Tamanho: %d bytes.", required_size);
    } else if (err == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGI(TAG, "Nenhum cadastro de animais (biz_json) encontrado na NVS.");
    } else {
        ESP_LOGE(TAG, "Erro ao acessar biz_json na NVS: %s", esp_err_to_name(err));
    }

    nvs_close(handle);
    return ESP_OK;
}

esp_err_t animal_db_lookup(const char *tag, animal_record_t *record_out) {
    if (tag == NULL || record_out == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    memset(record_out, 0, sizeof(animal_record_t));

    nvs_handle_t handle;
    esp_err_t err = nvs_open("bastao_biz", NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return ESP_ERR_NOT_FOUND;
    }

    size_t required_size = 0;
    err = nvs_get_str(handle, "biz_json", NULL, &required_size);
    if (err != ESP_OK) {
        nvs_close(handle);
        return ESP_ERR_NOT_FOUND;
    }

    char *json_str = malloc(required_size);
    if (json_str == NULL) {
        ESP_LOGE(TAG, "Falha ao alocar memória (%d bytes) para carregar o JSON de negócios.", required_size);
        nvs_close(handle);
        return ESP_ERR_NO_MEM;
    }

    err = nvs_get_str(handle, "biz_json", json_str, &required_size);
    nvs_close(handle);

    if (err != ESP_OK) {
        free(json_str);
        return ESP_ERR_NOT_FOUND;
    }

    cJSON *root = cJSON_Parse(json_str);
    free(json_str);

    if (root == NULL) {
        ESP_LOGE(TAG, "Erro ao parsear JSON de animais.");
        return ESP_FAIL;
    }

    if (!cJSON_IsArray(root)) {
        ESP_LOGE(TAG, "JSON de negócios não é um array válido.");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    bool found = false;
    int array_size = cJSON_GetArraySize(root);

    for (int i = 0; i < array_size; i++) {
        cJSON *item = cJSON_GetArrayItem(root, i);
        if (item == NULL) continue;

        cJSON *j_tag = cJSON_GetObjectItem(item, "tag");
        if (j_tag != NULL && cJSON_IsString(j_tag) && strcmp(j_tag->valuestring, tag) == 0) {
            // Animal encontrado! Copia os dados
            strncpy(record_out->tag, j_tag->valuestring, sizeof(record_out->tag) - 1);

            cJSON *j_name = cJSON_GetObjectItem(item, "name");
            if (j_name != NULL && cJSON_IsString(j_name)) {
                strncpy(record_out->name, j_name->valuestring, sizeof(record_out->name) - 1);
            }

            cJSON *j_weight = cJSON_GetObjectItem(item, "weight");
            if (j_weight != NULL && cJSON_IsNumber(j_weight)) {
                record_out->weight = (float)j_weight->valuedouble;
            }

            cJSON *j_lot = cJSON_GetObjectItem(item, "lot");
            if (j_lot != NULL && cJSON_IsString(j_lot)) {
                strncpy(record_out->lot, j_lot->valuestring, sizeof(record_out->lot) - 1);
            }

            found = true;
            break;
        }
    }

    cJSON_Delete(root);

    if (found) {
        ESP_LOGI(TAG, "Animal associado! Tag: %s, Nome: %s, Peso: %.2f kg, Lote: %s",
                 record_out->tag, record_out->name, record_out->weight, record_out->lot);
        return ESP_OK;
    }

    ESP_LOGD(TAG, "Tag %s não encontrada na base local.", tag);
    return ESP_ERR_NOT_FOUND;
}
