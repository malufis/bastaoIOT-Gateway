/**
 * @file wifi_driver.h
 * @brief Cabeçalho do driver Wi-Fi no modo Station (STA) do ESP32.
 * @details Este módulo expõe as APIs para inicialização do stack de Wi-Fi,
 *          gerenciamento da conexão com redes sem fio locais e controle do
 *          status de conexão.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#ifndef WIFI_DRIVER_H
#define WIFI_DRIVER_H

#include "esp_err.h"
#include <stdbool.h>

/**
 * @brief Inicializa o subsistema de rede Wi-Fi do ESP32 no modo Station (STA).
 * @details Cria a interface de rede padrão, configura o loop de eventos para
 *          reagir aos eventos da rede Wi-Fi e inicializa o hardware.
 * 
 * @return esp_err_t ESP_OK em caso de sucesso.
 */
esp_err_t wifi_driver_init(void);

/**
 * @brief Conecta o ESP32 a um ponto de acesso sem fio (Access Point).
 * 
 * @param[in] ssid Nome da rede Wi-Fi (SSID).
 * @param[in] password Senha da rede Wi-Fi (se necessário).
 * @return esp_err_t ESP_OK se a conexão foi disparada com sucesso.
 */
esp_err_t wifi_driver_connect(const char *ssid, const char *password);

/**
 * @brief Consulta se a rede Wi-Fi está atualmente conectada e ativa com IP válido.
 * 
 * @return true se o Wi-Fi estiver operacional com IP.
 */
bool wifi_driver_is_connected(void);

#endif // WIFI_DRIVER_H
