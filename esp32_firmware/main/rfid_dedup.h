#ifndef RFID_DEDUP_H
#define RFID_DEDUP_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdbool.h>
#include <string.h>

#define RFID_DEDUP_TABLE_SIZE   64
#define RFID_DEDUP_WINDOW_MS    60000
#define RFID_DEDUP_GPS_RADIUS_M 50.0f

typedef struct {
    uint32_t timestamp;
    char     tag[64];
    char     model[16];
    double   latitude;
    double   longitude;
    bool     has_gps;
    uint32_t read_count;
} rfid_dedup_entry_t;

void rfid_dedup_init(void);
bool rfid_dedup_is_duplicate(const char *tag, const char *model,
                              double lat, double lon, bool has_gps);
void rfid_dedup_clear(void);
uint32_t rfid_dedup_get_total_duplicates(void);
uint32_t rfid_dedup_get_total_unique(void);

#endif /* RFID_DEDUP_H */
