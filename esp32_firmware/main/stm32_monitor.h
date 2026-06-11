#ifndef STM32_MONITOR_H
#define STM32_MONITOR_H

#include <stdint.h>
#include "esp_err.h"

#define MONITOR_DASHBOARD_INTERVAL_MS 30000

typedef struct {
    uint32_t total_bytes_rx;
    uint32_t total_messages;
    uint32_t rfid_yrm100;
    uint32_t rfid_wl134;
    uint32_t battery;
    uint32_t alert;
    uint32_t heartbeat;
    uint32_t accel;
    uint32_t parse_errors;
    uint32_t queue_drops;
    uint32_t buffer_overflows;
    uint32_t unknown_types;
    uint32_t stm32_alive_checked;
    uint32_t stm32_dead_events;
} stm32_monitor_stats_t;

esp_err_t stm32_monitor_init(void);
void stm32_monitor_count_byte(void);
void stm32_monitor_count_message(const char *type, const char *model);
void stm32_monitor_count_parse_error(void);
void stm32_monitor_count_queue_drop(void);
void stm32_monitor_count_buffer_overflow(void);
void stm32_monitor_count_unknown_type(void);
void stm32_monitor_count_stm32_dead(void);
void stm32_monitor_print_dashboard(void);
void stm32_monitor_reset(void);
const stm32_monitor_stats_t *stm32_monitor_get_stats(void);

#endif
