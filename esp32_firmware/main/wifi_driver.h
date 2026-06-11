/**
 * @file wifi_driver.h
 * @brief Cabecalho do driver Wi-Fi no modo Station (STA) do ESP32.
 * @details Este modulo expoe as APIs para inicializacao do stack de Wi-Fi,
 *          gerenciamento da conexao com redes sem fio locais e controle do
 *          status de conexao.
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
 * @details Cria a interface de rede padrao, configura o loop de eventos para
 *          reagir aos eventos da rede Wi-Fi e inicializa o hardware.
 * 
 * @return esp_err_t ESP_OK em caso de sucesso.
 */
esp_err_t wifi_driver_init(void);

/**
 * @brief Conecta o ESP32 a um ponto de acesso sem fio (Access Point).
 * 
 * @param[in] ssid Nome da rede Wi-Fi (SSID).
 * @param[in] password Senha da rede Wi-Fi (se necessario).
 * @return esp_err_t ESP_OK se a conexao foi disparada com sucesso.
 */
esp_err_t wifi_driver_connect(const char *ssid, const char *password);

/**
 * @brief Consulta se a rede Wi-Fi esta atualmente conectada e ativa com IP valido.
 * 
 * @return true se o Wi-Fi estiver operacional com IP.
 */
bool wifi_driver_is_connected(void);

/**
 * @brief Desconecta o ESP32 da rede Wi-Fi ativa.
 * 
 * @return esp_err_t ESP_OK se a desconexao foi disparada com sucesso.
 */
esp_err_t wifi_driver_disconnect(void);

#endif // WIFI_DRIVER_H
