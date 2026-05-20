/**
 * @file ota_manager.h
 * @brief Gerenciador de atualizações de firmware remotas (OTA) via HTTPS.
 * @details Este módulo expõe funções para disparo de atualizações de firmware,
 *          seleção automática da melhor interface de rede disponível (Wi-Fi vs 4G PPP)
 *          e rollback automático.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#ifndef OTA_MANAGER_H
#define OTA_MANAGER_H

#include "esp_err.h"

/**
 * @brief Inicializa o subsistema de OTA e verifica se o firmware atual é estável
 *        (necessário para cancelar rollback do bootloader).
 * 
 * @return esp_err_t ESP_OK em caso de sucesso.
 */
esp_err_t ota_manager_init(void);

/**
 * @brief Dispara a tarefa assíncrona do FreeRTOS para download e gravação de firmware OTA.
 * 
 * @param[in] bin_url URL HTTPS apontando para o binário de firmware (ex: "https://server.com/firmware.bin").
 * @return esp_err_t ESP_OK se a tarefa foi iniciada com sucesso.
 */
esp_err_t ota_manager_start(const char *bin_url);

#endif // OTA_MANAGER_H
