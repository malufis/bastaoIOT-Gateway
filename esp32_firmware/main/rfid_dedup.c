#include "rfid_dedup.h"
#include "esp_log.h"
#include <math.h>

static const char *TAG = "RFID_DEDUP";

static rfid_dedup_entry_t dedup_table[RFID_DEDUP_TABLE_SIZE];
static int next_index = 0;
static uint32_t total_duplicates = 0;
static uint32_t total_unique = 0;

static float gps_distance_m(double lat1, double lon1, double lat2, double lon2)
{
    static const double DEG_TO_RAD = 3.14159265358979323846 / 180.0;
    static const double METERS_PER_DEG_LAT = 111320.0;
    double avg_lat = (lat1 + lat2) / 2.0;
    double meters_per_deg_lon = METERS_PER_DEG_LAT * cos(avg_lat * DEG_TO_RAD);
    double dx = (lon2 - lon1) * meters_per_deg_lon;
    double dy = (lat2 - lat1) * METERS_PER_DEG_LAT;
    return (float)sqrt(dx * dx + dy * dy);
}

void rfid_dedup_init(void)
{
    memset(dedup_table, 0, sizeof(dedup_table));
    next_index = 0;
    ESP_LOGI(TAG, "RFID dedup inicializado: tabela=%d, janela=%dms, raio_gps=%.1fm",
             RFID_DEDUP_TABLE_SIZE, RFID_DEDUP_WINDOW_MS, RFID_DEDUP_GPS_RADIUS_M);
}

bool rfid_dedup_is_duplicate(const char *tag, const char *model,
                              double lat, double lon, bool has_gps)
{
    if (tag == NULL || tag[0] == '\0') {
        return false;
    }

    uint32_t now = xTaskGetTickCount();

    for (int i = 0; i < RFID_DEDUP_TABLE_SIZE; i++) {
        rfid_dedup_entry_t *e = &dedup_table[i];
        if (e->timestamp == 0) {
            continue;
        }
        if (strcmp(e->tag, tag) != 0) {
            continue;
        }

        uint32_t elapsed = now - e->timestamp;
        if (elapsed > pdMS_TO_TICKS(RFID_DEDUP_WINDOW_MS)) {
            continue;
        }

        if (has_gps && e->has_gps) {
            float dist = gps_distance_m(lat, lon, e->latitude, e->longitude);
            if (dist > RFID_DEDUP_GPS_RADIUS_M) {
                continue;
            }
            ESP_LOGD(TAG, "Duplicata: tag=%s, delta_t=%lums, dist_gps=%.1fm",
                     tag, (unsigned long)(elapsed * portTICK_PERIOD_MS), dist);
        } else {
            ESP_LOGD(TAG, "Duplicata: tag=%s, delta_t=%lums (sem GPS)",
                     tag, (unsigned long)(elapsed * portTICK_PERIOD_MS));
        }

        e->read_count++;
        total_duplicates++;
        ESP_LOGI(TAG, "Duplicata: tag=%s, modelo=%s, vez=%lu, total_dups=%lu",
                 tag, e->model, (unsigned long)e->read_count, (unsigned long)total_duplicates);

        return true;
    }

    rfid_dedup_entry_t *slot = &dedup_table[next_index];
    slot->timestamp = now;
    slot->has_gps = has_gps;
    slot->latitude = lat;
    slot->longitude = lon;
    strncpy(slot->tag, tag, sizeof(slot->tag) - 1);
    slot->tag[sizeof(slot->tag) - 1] = '\0';
    if (model != NULL) {
        strncpy(slot->model, model, sizeof(slot->model) - 1);
        slot->model[sizeof(slot->model) - 1] = '\0';
    } else {
        slot->model[0] = '\0';
    }
    slot->read_count = 1;
    total_unique++;

    next_index = (next_index + 1) % RFID_DEDUP_TABLE_SIZE;
    return false;
}

void rfid_dedup_clear(void)
{
    memset(dedup_table, 0, sizeof(dedup_table));
    next_index = 0;
    total_duplicates = 0;
    total_unique = 0;
    ESP_LOGI(TAG, "Tabela de dedup limpa.");
}

uint32_t rfid_dedup_get_total_duplicates(void)
{
    return total_duplicates;
}

uint32_t rfid_dedup_get_total_unique(void)
{
    return total_unique;
}
