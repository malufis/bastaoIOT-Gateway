#ifndef WIFI_DRIVER_H
#define WIFI_DRIVER_H

#include "esp_err.h"
#include <stdbool.h>

esp_err_t wifi_driver_init(void);
esp_err_t wifi_driver_connect(const char *ssid, const char *password);
bool wifi_driver_is_connected(void);
esp_err_t wifi_driver_disconnect(void);
const char *wifi_driver_get_ap_ip(void);

#endif // WIFI_DRIVER_H
