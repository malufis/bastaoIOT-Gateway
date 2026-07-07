#pragma once

#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TAG_DB_MAX_TAGS_PER_DAY  500
#define TAG_DB_MAX_TAG_LEN       64
#define TAG_DB_MAX_NAME_LEN      64

/**
 * @brief Registro de uma tag lida.
 */
typedef struct {
    char tag[TAG_DB_MAX_TAG_LEN];
    char animal_name[TAG_DB_MAX_NAME_LEN];
    uint32_t timestamp_epoch;   /* epoch do momento da leitura */
    float latitude;
    float longitude;
    bool has_gps;
} tag_record_t;

/**
 * @brief Resumo do dia atual.
 */
typedef struct {
    char date[11];              /* "YYYY-MM-DD" */
    uint32_t total_reads;       /* total de leituras hoje */
    uint32_t unique_tags;       /* tags unicas hoje */
} tag_day_summary_t;

/**
 * @brief Inicializa o banco de dados de tags com SPIFFS.
 *
 * Monta SPIFFS e carrega/cria o arquivo do dia atual.
 * Deve ser chamado uma vez no boot.
 *
 * @return ESP_OK em caso de sucesso
 */
esp_err_t tag_database_init(void);

/**
 * @brief Registra uma tag lida.
 *
 * Adiciona a tag ao arquivo do dia atual. Se a tag ja existe,
 * incrementa o contador de leituras.
 *
 * @param[in] tag_id        Identificador da tag (ex: "900000000000123")
 * @param[in] animal_name   Nome do animal (pode ser NULL ou "")
 * @param[in] timestamp     Epoch timestamp (0 = usa horario do sistema)
 * @param[in] lat           Latitude (0.0 = sem GPS)
 * @param[in] lon           Longitude (0.0 = sem GPS)
 *
 * @return ESP_OK em caso de sucesso
 */
esp_err_t tag_database_add(const char *tag_id, const char *animal_name,
                           uint32_t timestamp, float lat, float lon);

/**
 * @brief Retorna o total de leituras de hoje.
 */
uint32_t tag_database_get_total_today(void);

/**
 * @brief Retorna o numero de tags unicas de hoje.
 */
uint32_t tag_database_get_unique_today(void);

/**
 * @brief Retorna o resumo do dia atual.
 */
tag_day_summary_t tag_database_get_summary(void);

/**
 * @brief Retorna um registro por indice (0 = mais recente).
 *
 * @param[in] index     Indice do registro (0-based)
 * @param[out] record   Preenchido com os dados do registro
 *
 * @return ESP_OK se existe, ESP_ERR_NOT_FOUND se index fora do limite
 */
esp_err_t tag_database_get_record(uint32_t index, tag_record_t *record);

/**
 * @brief Retorna o numero total de registros de hoje.
 */
uint32_t tag_database_get_record_count(void);

/**
 * @brief Salva o arquivo atual no SPIFFS.
 *
 * Chamado automaticamente a cada add, mas pode ser chamado manualmente.
 */
esp_err_t tag_database_flush(void);

/**
 * @brief Forca troca de dia (reset do contador).
 *
 * Usado quando o relogio do sistema e sincronizado e detecta mudanca de dia.
 */
void tag_database_new_day(void);

#ifdef __cplusplus
}
#endif
