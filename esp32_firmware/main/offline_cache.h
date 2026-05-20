/**
 * @file offline_cache.h
 * @brief Gerenciador de cache offline e spooler FIFO na flash local (SPIFFS).
 * @details Este módulo gerencia o salvamento persistente de payloads JSON criptografados
 *          quando não há conectividade com a rede/broker MQTT, permitindo posterior
 *          sincronização.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#ifndef OFFLINE_CACHE_H
#define OFFLINE_CACHE_H

#include "esp_err.h"
#include <stdbool.h>
#include <stddef.h>

#define SPIFFS_BASE_PATH "/spiffs"
#define CACHE_FILE_PREFIX "/spiffs/msg_"
#define CACHE_FILE_SUFFIX ".json"

/**
 * @brief Inicializa o sistema de arquivos SPIFFS e calcula os índices FIFO de leitura/escrita
 *        escaneando os arquivos existentes.
 * 
 * @return esp_err_t ESP_OK em caso de sucesso, ou código de erro pertinente.
 */
esp_err_t offline_cache_init(void);

/**
 * @brief Salva um payload no cache offline (fim da fila FIFO).
 * 
 * @param[in] payload String contendo o payload serializado (JSON criptografado em hex).
 * @return esp_err_t ESP_OK em caso de sucesso, ESP_ERR_NO_MEM se a memória estiver cheia (>95%),
 *                   ou outro código de erro se a escrita falhar.
 */
esp_err_t offline_cache_write(const char *payload);

/**
 * @brief Obtém o payload mais antigo do cache offline (início da fila FIFO).
 * 
 * @param[out] payload_out Buffer para armazenar o payload lido.
 * @param[in] max_len Tamanho máximo do buffer payload_out.
 * @return esp_err_t ESP_OK se encontrou e leu, ESP_ERR_NOT_FOUND se o cache estiver vazio,
 *                   ou outro código de erro em caso de falha de leitura.
 */
esp_err_t offline_cache_read_next(char *payload_out, size_t max_len);

/**
 * @brief Remove o payload mais antigo da fila offline (consome do FIFO).
 * 
 * @return esp_err_t ESP_OK se removeu com sucesso, ou erro se falhou.
 */
esp_err_t offline_cache_pop(void);

/**
 * @brief Verifica se o cache offline está vazio.
 * 
 * @return true Se não há elementos no cache.
 * @return false Se existem elementos a serem lidos e sincronizados.
 */
bool offline_cache_is_empty(void);

/**
 * @brief Inicia a tarefa assíncrona do FreeRTOS encarregada de sincronizar os dados do cache
 *        com o broker MQTT quando a rede estiver conectada.
 * 
 * @param[in] priority Prioridade da task.
 * @return esp_err_t ESP_OK em caso de sucesso, ou erro.
 */
esp_err_t offline_cache_sync_task_start(int priority);

#endif // OFFLINE_CACHE_H
