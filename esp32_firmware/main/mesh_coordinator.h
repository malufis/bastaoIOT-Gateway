/**
 * @file mesh_coordinator.h
 * @brief Cabeçalho do módulo coordenador e provisionador da rede local BLE Mesh.
 * @details Este módulo expõe a interface pública para inicializar a rede de malha BLE,
 *          definindo o ESP32 como provisionador seguro e gerenciando o envio de
 *          payloads criptografados para a Tela K10.
 * 
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#ifndef MESH_COORDINATOR_H
#define MESH_COORDINATOR_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Inicializa o subsistema Bluetooth, a pilha BLE Mesh e define a whitelist de UUIDs.
 * @details Realiza a carga dos drivers do controlador BLE do ESP32 e configura a pilha
 *          de malha (Mesh stack) no papel de Provisionador.
 * 
 * @pre O driver de NVS do ESP32 já deve ter sido previamente inicializado.
 * @post O ESP32 inicia a escuta por novos nós não-provisionados na rede (como a Tela K10).
 * 
 * @return esp_err_t ESP_OK em caso de sucesso, ou erro de driver específico.
 */
esp_err_t mesh_coordinator_init(void);

/**
 * @brief Envia um payload criptografado via BLE Mesh para os nós cadastrados (ex: Tela K10).
 * @details Empacota o payload em pacotes segmentados adequados ao protocolo Mesh e despacha
 *          de forma assíncrona.
 * 
 * @param[in] hex_payload String C terminada em '\0' contendo o payload em hexadecimal.
 * 
 * @pre A pilha BLE Mesh deve estar operacional (mesh_coordinator_init concluído).
 * @post A transmissão é agendada no loop de execução do BLE.
 * 
 * @return esp_err_t ESP_OK se o envio foi agendado com sucesso.
 *                   ESP_ERR_INVALID_STATE se a pilha Mesh estiver inativa.
 */
esp_err_t mesh_coordinator_send_data(const char *hex_payload);

#ifdef __cplusplus
}
#endif

#endif /* MESH_COORDINATOR_H */
