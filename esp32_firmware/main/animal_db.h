/**
 * @file animal_db.h
 * @brief Cabeçalho do banco de dados local de animais do Bastão-ESP.
 * @details Este módulo é responsável por carregar e realizar buscas na
 *          base de dados de negócios (Fazenda, Lote, Animal) persistida na NVS.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#ifndef ANIMAL_DB_H
#define ANIMAL_DB_H

#include "esp_err.h"
#include <stdbool.h>

#define ANIMAL_TAG_MAX_LEN 32
#define ANIMAL_NAME_MAX_LEN 64
#define ANIMAL_LOT_MAX_LEN 64

/**
 * @brief Estrutura representando um registro de animal recuperado.
 */
typedef struct {
    char tag[ANIMAL_TAG_MAX_LEN];    /**< Código da tag RFID */
    char name[ANIMAL_NAME_MAX_LEN];  /**< Nome ou brinco do animal */
    float weight;                     /**< Peso registrado do animal */
    char lot[ANIMAL_LOT_MAX_LEN];    /**< Nome do Lote/Retiro */
} animal_record_t;

/**
 * @brief Inicializa o módulo do banco de dados de animais local.
 * @details Verifica se a partição NVS contém dados de negócio válidos
 *          e loga as informações encontradas.
 * 
 * @return esp_err_t ESP_OK em caso de sucesso.
 */
esp_err_t animal_db_init(void);

/**
 * @brief Realiza a busca de um animal no banco de dados local com base na tag.
 * @details Abre a NVS, lê a string JSON de negócios, faz o parse usando cJSON
 *          e pesquisa a tag. Libera todos os recursos dinâmicos antes de retornar.
 * 
 * @param[in] tag String contendo a tag a ser pesquisada.
 * @param[out] record_out Ponteiro para a estrutura que receberá os dados se encontrado.
 * @return esp_err_t ESP_OK se encontrado, ESP_ERR_NOT_FOUND se não encontrado.
 */
esp_err_t animal_db_lookup(const char *tag, animal_record_t *record_out);

#endif // ANIMAL_DB_H
