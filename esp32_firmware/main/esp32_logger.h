/**
 * @file esp32_logger.h
 * @brief Módulo de logging wireless para debug remoto
 *
 * Permite visualização de logs via Telnet (Wi-Fi) e BLE GATT.
 * Útil para debug durante desenvolvimento em campo.
 */
#ifndef ESP32_LOGGER_H
#define ESP32_LOGGER_H

#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_BUFFER_SIZE 2048
#define LOG_TELNET_PORT 23
#define LOG_MAX_CLIENTS 3

typedef enum {
    LOG_LEVEL_VERBOSE = 0,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} log_level_t;

typedef struct {
    uint32_t timestamp_ms;
    log_level_t level;
    char tag[32];
    char message[256];
} log_entry_t;

esp_err_t esp_logger_init(void);
esp_err_t esp_logger_start_telnet(void);
esp_err_t esp_logger_start_ble(void);
esp_err_t esp_logger_write(log_level_t level, const char *tag, const char *message);
esp_err_t esp_logger_set_level(log_level_t level);
log_level_t esp_logger_get_level(void);
bool esp_logger_is_enabled(void);
void esp_logger_enable(bool enable);
uint32_t esp_logger_get_msg_count(void);
void esp_logger_clear_buffer(void);

esp_err_t esp_logger_dump_to_file(const char *path);

#ifdef __cplusplus
}
#endif

#endif // ESP32_LOGGER_H