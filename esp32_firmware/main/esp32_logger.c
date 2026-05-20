/**
 * @file esp32_logger.c
 * @brief Implementação do módulo de logging wireless para debug remoto
 */
#include "esp32_logger.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "lwip/sockets.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

static const char *TAG = "ESP_LOGGER";

static bool logger_enabled = true;
static log_level_t current_log_level = LOG_LEVEL_DEBUG;
static uint32_t msg_count = 0;
static uint32_t dropped_count = 0;

static QueueHandle_t log_queue = NULL;
static TaskHandle_t telnet_task_handle = NULL;
static TaskHandle_t ble_task_handle = NULL;

static int telnet_server_sock = -1;
static int telnet_client_socks[LOG_MAX_CLIENTS];
static volatile bool telnet_running = false;

static char log_buffer[LOG_BUFFER_SIZE];
static uint32_t log_buffer_head = 0;
static uint32_t log_buffer_count = 0;

static const char *level_strings[] = {
    "V", "D", "I", "W", "E"
};

static const char *level_color_escape[] = {
    "\033[37m",
    "\033[36m",
    "\033[32m",
    "\033[33m",
    "\033[31m"
};

static const char *level_reset = "\033[0m";

static void format_log_entry(char *buf, size_t buf_size, const log_entry_t *entry) {
    uint32_t secs = entry->timestamp_ms / 1000;
    uint32_t ms = entry->timestamp_ms % 1000;
    uint32_t hours = (secs / 3600) % 24;
    uint32_t mins = (secs % 3600) / 60;
    uint32_t sec = secs % 60;

    snprintf(buf, buf_size,
             "[%02u:%02u:%02u.%03u][%s%s%s][%s] %s",
             hours, mins, sec, ms,
             level_color_escape[entry->level],
             level_strings[entry->level],
             level_reset,
             entry->tag,
             entry->message);
}

static void buffer_log_entry(const log_entry_t *entry) {
    size_t entry_size = sizeof(log_entry_t);
    uint32_t next_head = (log_buffer_head + entry_size) % LOG_BUFFER_SIZE;

    if (next_head <= log_buffer_head ||
        log_buffer_count * entry_size < LOG_BUFFER_SIZE - entry_size) {
        memcpy(&log_buffer[log_buffer_head], entry, entry_size);
        log_buffer_head = next_head;
        log_buffer_count++;
    } else {
        log_buffer_head = 0;
        log_buffer_count = 0;
        memcpy(&log_buffer[0], entry, entry_size);
        log_buffer_head = entry_size;
        log_buffer_count = 1;
    }
}

static void broadcast_to_telnet(const char *formatted_msg) {
    int msg_len = strlen(formatted_msg);

    for (int i = 0; i < LOG_MAX_CLIENTS; i++) {
        if (telnet_client_socks[i] >= 0) {
            send(telnet_client_socks[i], formatted_msg, msg_len, 0);
        }
    }
}

static void broadcast_to_ble(const char *formatted_msg) {
    // Será integrado com ble_mobile_notify_log() posteriormente
    ESP_LOGD(TAG, "[BLE_LOG] %s", formatted_msg);
}

esp_err_t esp_logger_init(void) {
    log_queue = xQueueCreate(64, sizeof(log_entry_t));
    if (log_queue == NULL) {
        ESP_LOGE(TAG, "Falha ao criar fila de logs");
        return ESP_FAIL;
    }

    for (int i = 0; i < LOG_MAX_CLIENTS; i++) {
        telnet_client_socks[i] = -1;
    }

    log_buffer_head = 0;
    log_buffer_count = 0;
    msg_count = 0;
    dropped_count = 0;

    ESP_LOGI(TAG, "Logger wireless inicializado. Nivel: %s",
             level_strings[current_log_level]);
    return ESP_OK;
}

static void telnet_task(void *pvParameters) {
    ESP_LOGI(TAG, "Task Telnet iniciada na porta %d", LOG_TELNET_PORT);

    telnet_server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (telnet_server_sock < 0) {
        ESP_LOGE(TAG, "Falha ao criar socket Telnet");
        telnet_task_handle = NULL;
        vTaskDelete(NULL);
        return;
    }

    int opt = 1;
    setsockopt(telnet_server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(LOG_TELNET_PORT)
    };

    if (bind(telnet_server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        ESP_LOGE(TAG, "Falha ao fazer bind na porta Telnet");
        close(telnet_server_sock);
        telnet_task_handle = NULL;
        vTaskDelete(NULL);
        return;
    }

    if (listen(telnet_server_sock, LOG_MAX_CLIENTS) < 0) {
        ESP_LOGE(TAG, "Falha ao escutar na porta Telnet");
        close(telnet_server_sock);
        telnet_task_handle = NULL;
        vTaskDelete(NULL);
        return;
    }

    telnet_running = true;

    while (telnet_running) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_sock = accept(telnet_server_sock, (struct sockaddr *)&client_addr, &addr_len);

        if (client_sock < 0) {
            if (telnet_running) {
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
            continue;
        }

        int free_slot = -1;
        for (int i = 0; i < LOG_MAX_CLIENTS; i++) {
            if (telnet_client_socks[i] < 0) {
                free_slot = i;
                break;
            }
        }

        if (free_slot >= 0) {
            telnet_client_socks[free_slot] = client_sock;
            char *welcome = "\r\n=== Bastão-ESP Telnet Debug ===\r\nDigite 'help' para comandos\r\n\r\n";
            send(client_sock, welcome, strlen(welcome), 0);

            char client_ip[16];
            inet_ntoa_r(client_addr.sin_addr, client_ip, sizeof(client_ip));
            ESP_LOGI(TAG, "Cliente Telnet conectado: %s", client_ip);
        } else {
            char *full = "Numero maximo de clientes conectado\r\n";
            send(client_sock, full, strlen(full), 0);
            close(client_sock);
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    for (int i = 0; i < LOG_MAX_CLIENTS; i++) {
        if (telnet_client_socks[i] >= 0) {
            close(telnet_client_socks[i]);
            telnet_client_socks[i] = -1;
        }
    }

    if (telnet_server_sock >= 0) {
        close(telnet_server_sock);
        telnet_server_sock = -1;
    }

    telnet_task_handle = NULL;
    vTaskDelete(NULL);
}

esp_err_t esp_logger_start_telnet(void) {
    if (telnet_task_handle != NULL) {
        ESP_LOGW(TAG, "Telnet ja esta em execucao");
        return ESP_OK;
    }

    BaseType_t ret = xTaskCreate(telnet_task, "telnet_logger", 4096, NULL, 3, &telnet_task_handle);
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Falha ao criar task Telnet");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Servidor Telnet iniciado na porta %d", LOG_TELNET_PORT);
    return ESP_OK;
}

static void log_processor_task(void *pvParameters) {
    log_entry_t entry;
    char formatted_msg[384];

    ESP_LOGI(TAG, "Processador de logs iniciado");

    while (1) {
        if (xQueueReceive(log_queue, &entry, pdMS_TO_TICKS(100)) == pdTRUE) {
            format_log_entry(formatted_msg, sizeof(formatted_msg), &entry);

            broadcast_to_telnet(formatted_msg);
            broadcast_to_telnet("\r\n");

            buffer_log_entry(&entry);
            msg_count++;
        }
    }
}

esp_err_t esp_logger_write(log_level_t level, const char *tag, const char *message) {
    if (!logger_enabled || level < current_log_level) {
        return ESP_OK;
    }

    log_entry_t entry = {
        .timestamp_ms = esp_log_timestamp(),
        .level = level,
        .message = {0}
    };

    strncpy(entry.tag, tag, sizeof(entry.tag) - 1);
    strncpy(entry.message, message, sizeof(entry.message) - 1);

    if (log_queue != NULL) {
        if (xQueueSend(log_queue, &entry, 0) != pdTRUE) {
            dropped_count++;
            ESP_LOGW(TAG, "Log descartado (fila cheia): %s", message);
        }
    }

    return ESP_OK;
}

esp_err_t esp_logger_set_level(log_level_t level) {
    if (level > LOG_LEVEL_ERROR) {
        return ESP_ERR_INVALID_ARG;
    }
    current_log_level = level;
    ESP_LOGI(TAG, "Nivel de log alterado para: %s", level_strings[level]);
    return ESP_OK;
}

log_level_t esp_logger_get_level(void) {
    return current_log_level;
}

bool esp_logger_is_enabled(void) {
    return logger_enabled;
}

void esp_logger_enable(bool enable) {
    logger_enabled = enable;
    ESP_LOGI(TAG, "Logger %s", enable ? "habilitado" : "desabilitado");
}

uint32_t esp_logger_get_msg_count(void) {
    return msg_count;
}

uint32_t esp_logger_get_dropped_count(void) {
    return dropped_count;
}

void esp_logger_clear_buffer(void) {
    log_buffer_head = 0;
    log_buffer_count = 0;
    msg_count = 0;
    dropped_count = 0;
    ESP_LOGI(TAG, "Buffer de logs limpo");
}

esp_err_t esp_logger_dump_to_file(const char *path) {
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Falha ao abrir arquivo para dump: %s", path);
        return ESP_FAIL;
    }

    log_entry_t entry;
    char formatted[384];
    size_t entry_size = sizeof(log_entry_t);

    fprintf(f, "=== Bastão-ESP Log Dump ===\n");
    fprintf(f, "Total mensagens: %lu, Descartadas: %lu\n\n",
            (unsigned long)msg_count, (unsigned long)dropped_count);

    for (uint32_t i = 0; i < log_buffer_count; i++) {
        uint32_t idx = (log_buffer_head + entry_size * i) % LOG_BUFFER_SIZE;
        memcpy(&entry, &log_buffer[idx], entry_size);
        format_log_entry(formatted, sizeof(formatted), &entry);
        fprintf(f, "%s\n", formatted);
    }

    fclose(f);
    ESP_LOGI(TAG, "Logs salvos em: %s", path);
    return ESP_OK;
}

esp_err_t esp_logger_start_ble(void) {
    ESP_LOGI(TAG, "BLE logging disponivel via GATT 0xFF07");
    return ESP_OK;
}