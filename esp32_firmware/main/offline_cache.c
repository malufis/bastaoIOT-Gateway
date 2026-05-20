/**
 * @file offline_cache.c
 * @brief Implementação do gerenciador de cache offline e spooler FIFO.
 * @details Este módulo implementa a montagem de partição SPIFFS, a busca por
 *          arquivos órfãos para inicialização dos ponteiros de leitura/escrita,
 *          o controle de capacidade (limite de 95%) e a tarefa de sincronização.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#include "offline_cache.h"
#include "esp_spiffs.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt_publisher.h"
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>

static const char *TAG = "OFFLINE_CACHE";

/* --- Variáveis de Estado Interno --- */
static int read_index = 0;  /**< Índice do arquivo de leitura mais antigo (FIFO) */
static int write_index = 0; /**< Índice para a gravação do próximo arquivo (FIFO) */
static bool is_spiffs_mounted = false;

/* --- Declaração de Funções Privadas --- */
static void offline_cache_sync_task(void *pvParameters);

esp_err_t offline_cache_init(void) {
    ESP_LOGI(TAG, "Inicializando o sistema de arquivos SPIFFS...");

    esp_vfs_spiffs_conf_t conf = {
        .base_path = SPIFFS_BASE_PATH,
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Falha ao montar ou formatar o SPIFFS.");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Partição SPIFFS não encontrada no mapa de partições.");
        } else {
            ESP_LOGE(TAG, "Falha na inicialização do SPIFFS (%s).", esp_err_to_name(ret));
        }
        return ret;
    }

    is_spiffs_mounted = true;

    // Mede a capacidade inicial do SPIFFS
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "SPIFFS montado. Total: %d bytes, Usado: %d bytes (%.1f%%).",
                 total, used, (double)used / total * 100.0);
    }

    // Escaneia a pasta para descobrir arquivos e reestabelecer o índice FIFO
    DIR *dir = opendir(SPIFFS_BASE_PATH);
    if (!dir) {
        ESP_LOGE(TAG, "Falha ao abrir diretório %s para escanear cache.", SPIFFS_BASE_PATH);
        return ESP_FAIL;
    }

    struct dirent *entry;
    int min_idx = -1;
    int max_idx = -1;

    while ((entry = readdir(dir)) != NULL) {
        int idx;
        // Espera arquivos com o formato "msg_XXXXX.json"
        if (sscanf(entry->d_name, "msg_%d.json", &idx) == 1) {
            if (min_idx == -1 || idx < min_idx) {
                min_idx = idx;
            }
            if (max_idx == -1 || idx > max_idx) {
                max_idx = idx;
            }
        }
    }
    closedir(dir);

    if (min_idx != -1) {
        read_index = min_idx;
        write_index = max_idx + 1;
        ESP_LOGI(TAG, "Arquivos de cache recuperados. read_index: %d, write_index: %d, pendentes: %d",
                 read_index, write_index, (write_index - read_index));
    } else {
        read_index = 0;
        write_index = 0;
        ESP_LOGI(TAG, "Nenhum arquivo de cache existente encontrado.");
    }

    return ESP_OK;
}

esp_err_t offline_cache_write(const char *payload) {
    if (!is_spiffs_mounted) {
        return ESP_ERR_INVALID_STATE;
    }

    if (payload == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    // Verifica limite de 95% do armazenamento
    size_t total = 0, used = 0;
    esp_err_t ret = esp_spiffs_info(NULL, &total, &used);
    if (ret == ESP_OK) {
        if (used >= (total * 95 / 100)) {
            ESP_LOGE(TAG, "Espaço em flash crítico (>=95%%). Escrita no cache bloqueada!");
            // Sinalização audível/visual seria acionada aqui
            // Ex: buzzer_trigger_alert();
            return ESP_ERR_NO_MEM;
        }
    }

    char filepath[64];
    snprintf(filepath, sizeof(filepath), "%s%d%s", CACHE_FILE_PREFIX, write_index, CACHE_FILE_SUFFIX);

    FILE *f = fopen(filepath, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Falha ao criar arquivo de cache: %s", filepath);
        return ESP_FAIL;
    }

    int bytes_written = fprintf(f, "%s", payload);
    fclose(f);

    if (bytes_written < 0) {
        ESP_LOGE(TAG, "Falha ao escrever dados no arquivo de cache.");
        unlink(filepath);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Mensagem persistida no cache. Arquivo: %s", filepath);
    write_index++;

    return ESP_OK;
}

esp_err_t offline_cache_read_next(char *payload_out, size_t max_len) {
    if (!is_spiffs_mounted) {
        return ESP_ERR_INVALID_STATE;
    }

    if (payload_out == NULL || max_len == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    if (read_index >= write_index) {
        return ESP_ERR_NOT_FOUND;
    }

    char filepath[64];
    snprintf(filepath, sizeof(filepath), "%s%d%s", CACHE_FILE_PREFIX, read_index, CACHE_FILE_SUFFIX);

    FILE *f = fopen(filepath, "r");
    if (f == NULL) {
        ESP_LOGW(TAG, "Arquivo de leitura ausente ou corrompido: %s", filepath);
        return ESP_ERR_NOT_FOUND;
    }

    // Lê até preencher o buffer ou até EOF
    size_t bytes_read = fread(payload_out, 1, max_len - 1, f);
    payload_out[bytes_read] = '\0';
    fclose(f);

    return ESP_OK;
}

esp_err_t offline_cache_pop(void) {
    if (!is_spiffs_mounted) {
        return ESP_ERR_INVALID_STATE;
    }

    if (read_index >= write_index) {
        return ESP_ERR_NOT_FOUND;
    }

    char filepath[64];
    snprintf(filepath, sizeof(filepath), "%s%d%s", CACHE_FILE_PREFIX, read_index, CACHE_FILE_SUFFIX);

    // Remove o arquivo físico
    if (unlink(filepath) != 0) {
        ESP_LOGE(TAG, "Erro ao remover arquivo de cache: %s", filepath);
    } else {
        ESP_LOGD(TAG, "Arquivo de cache removido: %s", filepath);
    }

    read_index++;

    // Reinicia índices para evitar crescer indefinidamente se o buffer esvaziar
    if (read_index == write_index) {
        ESP_LOGI(TAG, "Cache de dados esvaziado. Reiniciando índices.");
        read_index = 0;
        write_index = 0;
    }

    return ESP_OK;
}

bool offline_cache_is_empty(void) {
    return (read_index >= write_index);
}

esp_err_t offline_cache_sync_task_start(int priority) {
    BaseType_t ret = xTaskCreate(offline_cache_sync_task, "cache_sync_task", 4096, NULL, priority, NULL);
    return (ret == pdPASS) ? ESP_OK : ESP_FAIL;
}

/**
 * @brief Task FreeRTOS de monitoramento e sincronização em background.
 * @details Executa ciclicamente e, ao detectar conexão MQTT ativa, consome
 *          as mensagens pendentes no cache FIFO local e as enfileira para envio.
 */
static void offline_cache_sync_task(void *pvParameters) {
    char buf[MQTT_PAYLOAD_MAX_LEN];
    ESP_LOGI(TAG, "Task de sincronização de cache iniciada.");

    while (1) {
        // Só tenta descarregar se estivermos conectados ao MQTT e o cache tiver mensagens
        if (mqtt_publisher_is_connected() && !offline_cache_is_empty()) {
            ESP_LOGI(TAG, "Conexão restabelecida. Iniciando escoamento do cache offline...");

            while (mqtt_publisher_is_connected() && !offline_cache_is_empty()) {
                memset(buf, 0, sizeof(buf));
                esp_err_t err = offline_cache_read_next(buf, sizeof(buf));
                if (err == ESP_OK) {
                    // Envia para o tópico de telemetria
                    err = mqtt_publisher_enqueue("bastao/telemetria", buf, 1);
                    if (err == ESP_OK) {
                        // Remove do cache apenas após enfileirar com sucesso
                        offline_cache_pop();
                        // Aguarda um pequeno delay para não saturar a fila local do MQTT
                        vTaskDelay(200 / portTICK_PERIOD_MS);
                    } else {
                        ESP_LOGW(TAG, "Falha ao reinserir mensagem no publicador MQTT. Tentando novamente mais tarde.");
                        break;
                    }
                } else if (err == ESP_ERR_NOT_FOUND) {
                    // Arquivo físico não pôde ser aberto (corrompido/inexistente)
                    // Avança o pop para não travar a fila
                    offline_cache_pop();
                } else {
                    ESP_LOGE(TAG, "Erro crítico ao ler próximo registro de cache: %d", err);
                    break;
                }
            }
            ESP_LOGI(TAG, "Escoamento de cache concluído ou interrompido.");
        }

        // Delay de verificação do status
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
