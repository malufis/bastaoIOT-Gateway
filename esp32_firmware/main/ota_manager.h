/**
 * @file ota_manager.h
 * @brief Gerenciador de atualizacoes de firmware remotas (OTA) via HTTPS.
 * @details Este modulo expoe funcoes para disparo de atualizacoes de firmware,
 *          selecao automatica da melhor interface de rede disponivel (Wi-Fi vs 4G PPP)
 *          e rollback automatico.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#ifndef OTA_MANAGER_H
#define OTA_MANAGER_H

#include "esp_err.h"

/**
 * @brief Inicializa o subsistema de OTA e verifica se o firmware atual e estavel
 *        (necessario para cancelar rollback do bootloader).
 * 
 * @return esp_err_t ESP_OK em caso de sucesso.
 */
esp_err_t ota_manager_init(void);

/**
 * @brief Dispara a tarefa assincrona do FreeRTOS para download e gravacao de firmware OTA.
 * 
 * @param[in] bin_url URL HTTPS apontando para o binario de firmware (ex: "https://server.com/firmware.bin").
 * @return esp_err_t ESP_OK se a tarefa foi iniciada com sucesso.
 */
esp_err_t ota_manager_start(const char *bin_url);

#endif // OTA_MANAGER_H
