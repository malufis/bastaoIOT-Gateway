#include "stm32_monitor.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "MONITOR";

static stm32_monitor_stats_t stats;
static uint32_t last_dashboard_tick = 0;
static uint32_t last_bytes_rx = 0;
static uint32_t last_messages = 0;
static TickType_t start_tick = 0;

static void print_line(const char *label, uint32_t value, const char *unit)
{
    if (value > 0) {
        ESP_LOGI(TAG, "  %-20s %5lu %s", label, (unsigned long)value, unit);
    } else {
        ESP_LOGI(TAG, "  %-20s    0 %s", label, unit);
    }
}

esp_err_t stm32_monitor_init(void)
{
    memset(&stats, 0, sizeof(stm32_monitor_stats_t));
    last_dashboard_tick = 0;
    last_bytes_rx = 0;
    last_messages = 0;
    start_tick = xTaskGetTickCount();
    ESP_LOGI(TAG, "Monitor STM32 UART iniciado");
    return ESP_OK;
}

void stm32_monitor_count_byte(void)
{
    stats.total_bytes_rx++;
}

void stm32_monitor_count_message(const char *type, const char *model)
{
    stats.total_messages++;

    if (strcmp(type, "rfid") == 0) {
        if (model != NULL && strcmp(model, "WL134") == 0) {
            stats.rfid_wl134++;
        } else {
            stats.rfid_yrm100++;
        }
    } else if (strcmp(type, "batt") == 0) {
        stats.battery++;
    } else if (strcmp(type, "alert") == 0) {
        stats.alert++;
    } else if (strcmp(type, "heartbeat") == 0) {
        stats.heartbeat++;
    } else if (strcmp(type, "accel") == 0) {
        stats.accel++;
    }

    uint32_t now = xTaskGetTickCount();
    if (now - last_dashboard_tick >= pdMS_TO_TICKS(MONITOR_DASHBOARD_INTERVAL_MS)) {
        last_dashboard_tick = now;
        stm32_monitor_print_dashboard();
        last_bytes_rx = stats.total_bytes_rx;
        last_messages = stats.total_messages;
    }
}

void stm32_monitor_count_parse_error(void)
{
    stats.parse_errors++;
}

void stm32_monitor_count_queue_drop(void)
{
    stats.queue_drops++;
}

void stm32_monitor_count_buffer_overflow(void)
{
    stats.buffer_overflows++;
}

void stm32_monitor_count_unknown_type(void)
{
    stats.unknown_types++;
}

void stm32_monitor_count_stm32_dead(void)
{
    stats.stm32_dead_events++;
}

void stm32_monitor_print_dashboard(void)
{
    TickType_t elapsed_ticks = xTaskGetTickCount() - start_tick;
    uint32_t elapsed_min = elapsed_ticks > 0
        ? (uint32_t)(elapsed_ticks * portTICK_PERIOD_MS / 60000)
        : 1;
    if (elapsed_min == 0) elapsed_min = 1;

    uint32_t msg_per_min = stats.total_messages / elapsed_min;
    uint32_t bytes_per_min = stats.total_bytes_rx / elapsed_min;

    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "  STM32 UART MONITOR");
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "  Executando ha %lu min", (unsigned long)elapsed_min);
    print_line("Bytes RX", stats.total_bytes_rx, "");
    ESP_LOGI(TAG, "  ------------------------");
    print_line("Total msg", stats.total_messages, "");
    print_line("RFID YRM100", stats.rfid_yrm100, "");
    print_line("RFID WL134", stats.rfid_wl134, "");
    print_line("Bateria", stats.battery, "");
    print_line("Alerta", stats.alert, "");
    print_line("Heartbeat", stats.heartbeat, "");
    print_line("Acelerometro", stats.accel, "");
    ESP_LOGI(TAG, "  ------------------------");
    print_line("Erros parse", stats.parse_errors, "");
    print_line("Queue drops", stats.queue_drops, "");
    print_line("Buffer ovf", stats.buffer_overflows, "");
    print_line("Tipo desconh", stats.unknown_types, "");
    print_line("STM32 dead evt", stats.stm32_dead_events, "");
    ESP_LOGI(TAG, "  ------------------------");
    ESP_LOGI(TAG, "  Taxa: %lu msg/min | %lu bytes/min",
             (unsigned long)msg_per_min, (unsigned long)bytes_per_min);
    ESP_LOGI(TAG, "========================================");
}

void stm32_monitor_reset(void)
{
    memset(&stats, 0, sizeof(stm32_monitor_stats_t));
    last_dashboard_tick = xTaskGetTickCount();
    last_bytes_rx = 0;
    last_messages = 0;
    start_tick = xTaskGetTickCount();
    ESP_LOGI(TAG, "Contadores do monitor resetados");
}

const stm32_monitor_stats_t *stm32_monitor_get_stats(void)
{
    return &stats;
}
