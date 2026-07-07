#include "tag_database.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "cJSON.h"
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>

static const char *TAG = "TAG_DB";
static const char *MOUNT_POINT = "/spiffs";
static const char *DB_DIR = "/spiffs/tags";

/* ── Estado interno ──────────────────────────────── */
static char s_current_date[11] = {0};   /* "YYYY-MM-DD" */
static uint32_t s_total_reads = 0;
static uint32_t s_unique_count = 0;

/* Buffer em RAM dos registros do dia */
static tag_record_t s_records[TAG_DB_MAX_TAGS_PER_DAY];
static uint32_t s_record_count = 0;
static bool s_dirty = false;

/* ── Helpers ─────────────────────────────────────── */

/**
 * @brief Monta o path do arquivo do dia: /spiffs/tags/YYYY-MM-DD.json
 */
static void build_day_path(char *buf, size_t len, const char *date) {
    snprintf(buf, len, "%s/%s.json", DB_DIR, date);
}

/**
 * @brief Obtem a string de data atual (UTC-3 ou sistema).
 *
 * Se o timestamp_epoch for 0, usa time(NULL) com offset -3h.
 */
static void get_current_date(char *date_buf, size_t len) {
    time_t now;
    time(&now);
    /* Ajuste UTC-3 para horario de Brasilia */
    now -= 3 * 3600;
    struct tm *tm = gmtime(&now);
    snprintf(date_buf, len, "%04d-%02d-%02d",
             tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
}

/**
 * @brief Carrega o arquivo JSON do dia para o buffer RAM.
 */
static esp_err_t load_day_file(const char *date) {
    char path[64];
    build_day_path(path, sizeof(path), date);

    FILE *f = fopen(path, "r");
    if (!f) {
        ESP_LOGI(TAG, "Arquivo do dia %s nao existe, criando novo", date);
        s_record_count = 0;
        s_total_reads = 0;
        s_unique_count = 0;
        return ESP_OK;
    }

    /* Le o conteudo inteiro */
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (size <= 0) {
        fclose(f);
        s_record_count = 0;
        s_total_reads = 0;
        s_unique_count = 0;
        return ESP_OK;
    }

    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        fclose(f);
        return ESP_ERR_NO_MEM;
    }
    fread(json_str, 1, size, f);
    json_str[size] = '\0';
    fclose(f);

    /* Parse JSON */
    cJSON *root = cJSON_Parse(json_str);
    free(json_str);
    if (!root) {
        ESP_LOGE(TAG, "Erro ao parsear JSON do dia %s", date);
        s_record_count = 0;
        s_total_reads = 0;
        s_unique_count = 0;
        return ESP_FAIL;
    }

    s_total_reads = cJSON_GetObjectItem(root, "total_reads")->valueint;
    s_unique_count = cJSON_GetObjectItem(root, "unique_tags")->valueint;

    cJSON *tags = cJSON_GetObjectItem(root, "tags");
    s_record_count = cJSON_GetArraySize(tags);
    if (s_record_count > TAG_DB_MAX_TAGS_PER_DAY) {
        s_record_count = TAG_DB_MAX_TAGS_PER_DAY;
    }

    for (uint32_t i = 0; i < s_record_count; i++) {
        cJSON *item = cJSON_GetArrayItem(tags, i);
        tag_record_t *rec = &s_records[i];
        memset(rec, 0, sizeof(tag_record_t));

        const cJSON *tag = cJSON_GetObjectItem(item, "tag");
        const cJSON *name = cJSON_GetObjectItem(item, "name");
        const cJSON *ts = cJSON_GetObjectItem(item, "ts");
        const cJSON *lat = cJSON_GetObjectItem(item, "lat");
        const cJSON *lon = cJSON_GetObjectItem(item, "lon");

        if (tag) strncpy(rec->tag, tag->valuestring, TAG_DB_MAX_TAG_LEN - 1);
        if (name) strncpy(rec->animal_name, name->valuestring, TAG_DB_MAX_NAME_LEN - 1);
        if (ts) rec->timestamp_epoch = (uint32_t)ts->valuedouble;
        if (lat) rec->latitude = (float)lat->valuedouble;
        if (lon) rec->longitude = (float)lon->valuedouble;
        rec->has_gps = (rec->latitude != 0.0f || rec->longitude != 0.0f);
    }

    cJSON_Delete(root);
    ESP_LOGI(TAG, "Carregado dia %s: %lu leituras, %lu unicas, %lu registros",
             date, s_total_reads, s_unique_count, s_record_count);
    return ESP_OK;
}

/**
 * @brief Salva o buffer RAM para o arquivo JSON do dia.
 */
static esp_err_t save_day_file(void) {
    if (!s_dirty) return ESP_OK;

    char path[64];
    build_day_path(path, sizeof(path), s_current_date);

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "date", s_current_date);
    cJSON_AddNumberToObject(root, "total_reads", s_total_reads);
    cJSON_AddNumberToObject(root, "unique_tags", s_unique_count);

    cJSON *tags = cJSON_AddArrayToObject(root, "tags");
    for (uint32_t i = 0; i < s_record_count; i++) {
        cJSON *item = cJSON_CreateObject();
        cJSON_AddStringToObject(item, "tag", s_records[i].tag);
        cJSON_AddStringToObject(item, "name", s_records[i].animal_name);
        cJSON_AddNumberToObject(item, "ts", s_records[i].timestamp_epoch);
        cJSON_AddNumberToObject(item, "lat", s_records[i].latitude);
        cJSON_AddNumberToObject(item, "lon", s_records[i].longitude);
        cJSON_AddItemToArray(tags, item);
    }

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    FILE *f = fopen(path, "w");
    if (!f) {
        ESP_LOGE(TAG, "Falha ao abrir %s para escrita", path);
        free(json_str);
        return ESP_FAIL;
    }
    size_t len = strlen(json_str);
    fwrite(json_str, 1, len, f);
    fclose(f);
    free(json_str);

    s_dirty = false;
    ESP_LOGI(TAG, "Salvo %s: %zu bytes", path, len);
    return ESP_OK;
}

/* ── API publica ─────────────────────────────────── */

esp_err_t tag_database_init(void) {
    ESP_LOGI(TAG, "Inicializando SPIFFS...");

    esp_vfs_spiffs_conf_t spiffs_conf = {
        .base_path = MOUNT_POINT,
        .partition_label = "spiffs",
        .max_files = 8,
        .format_if_mount_failed = true,
    };

    esp_err_t ret = esp_vfs_spiffs_register(&spiffs_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao montar SPIFFS: %s", esp_err_to_name(ret));
        return ret;
    }

    size_t total = 0, used = 0;
    esp_spiffs_info("spiffs", &total, &used);
    ESP_LOGI(TAG, "SPIFFS: total=%zu used=%zu free=%zu", total, used, total - used);

    /* Cria diretorio de tags */
    mkdir(DB_DIR, 0755);

    /* Carrega dados do dia atual */
    get_current_date(s_current_date, sizeof(s_current_date));
    ESP_LOGI(TAG, "Data atual: %s", s_current_date);

    return load_day_file(s_current_date);
}

esp_err_t tag_database_add(const char *tag_id, const char *animal_name,
                           uint32_t timestamp, float lat, float lon) {
    if (!tag_id || strlen(tag_id) == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    /* Verifica se a data mudou */
    char today[11];
    get_current_date(today, sizeof(today));
    if (strcmp(today, s_current_date) != 0) {
        ESP_LOGI(TAG, "Mudanca de dia: %s -> %s", s_current_date, today);
        save_day_file();  /* Salva o dia anterior */
        strncpy(s_current_date, today, sizeof(s_current_date));
        load_day_file(s_current_date);
    }

    s_total_reads++;

    /* Verifica se a tag ja existe no dia */
    int existing_idx = -1;
    for (uint32_t i = 0; i < s_record_count; i++) {
        if (strcmp(s_records[i].tag, tag_id) == 0) {
            existing_idx = i;
            break;
        }
    }

    if (existing_idx >= 0) {
        /* Tag ja existe — so incrementa leitura total (ja feito) */
        ESP_LOGD(TAG, "Tag %s ja existe (idx=%d), atualizando timestamp", tag_id, existing_idx);
        s_records[existing_idx].timestamp_epoch = timestamp;
        if (lat != 0.0f || lon != 0.0f) {
            s_records[existing_idx].latitude = lat;
            s_records[existing_idx].longitude = lon;
            s_records[existing_idx].has_gps = true;
        }
    } else {
        /* Nova tag — adiciona */
        if (s_record_count >= TAG_DB_MAX_TAGS_PER_DAY) {
            ESP_LOGW(TAG, "Limite de %d registros atingido, descartando mais antigo",
                     TAG_DB_MAX_TAGS_PER_DAY);
            /* Desloca tudo para tras */
            memmove(&s_records[0], &s_records[1],
                    (TAG_DB_MAX_TAGS_PER_DAY - 1) * sizeof(tag_record_t));
            s_record_count = TAG_DB_MAX_TAGS_PER_DAY - 1;
        }

        tag_record_t *rec = &s_records[s_record_count];
        memset(rec, 0, sizeof(tag_record_t));
        strncpy(rec->tag, tag_id, TAG_DB_MAX_TAG_LEN - 1);
        if (animal_name) strncpy(rec->animal_name, animal_name, TAG_DB_MAX_NAME_LEN - 1);
        rec->timestamp_epoch = timestamp;
        rec->latitude = lat;
        rec->longitude = lon;
        rec->has_gps = (lat != 0.0f || lon != 0.0f);

        s_record_count++;
        s_unique_count++;

        ESP_LOGI(TAG, "Nova tag: %s (total=%lu, unicas=%lu)", tag_id, s_total_reads, s_unique_count);
    }

    s_dirty = true;

    /* Salva a cada 10 leituras para evitar writes frequentes */
    if (s_total_reads % 10 == 0) {
        return save_day_file();
    }
    return ESP_OK;
}

uint32_t tag_database_get_total_today(void) {
    return s_total_reads;
}

uint32_t tag_database_get_unique_today(void) {
    return s_unique_count;
}

tag_day_summary_t tag_database_get_summary(void) {
    tag_day_summary_t sum;
    memset(&sum, 0, sizeof(sum));
    strncpy(sum.date, s_current_date, sizeof(sum.date));
    sum.total_reads = s_total_reads;
    sum.unique_tags = s_unique_count;
    return sum;
}

esp_err_t tag_database_get_record(uint32_t index, tag_record_t *record) {
    if (index >= s_record_count || !record) {
        return ESP_ERR_NOT_FOUND;
    }
    *record = s_records[index];
    return ESP_OK;
}

uint32_t tag_database_get_record_count(void) {
    return s_record_count;
}

esp_err_t tag_database_flush(void) {
    return save_day_file();
}

void tag_database_new_day(void) {
    ESP_LOGI(TAG, "Forcando troca de dia");
    save_day_file();
    get_current_date(s_current_date, sizeof(s_current_date));
    load_day_file(s_current_date);
}
