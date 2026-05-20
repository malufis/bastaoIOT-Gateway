/**
 * @file secure_payload.h
 * @brief Cabeçalho do módulo de segurança e criptografia de payloads.
 * @details Este módulo define as rotinas de criptografia simétrica AES para
 *          blindagem de payloads de telemetria enviados via BLE Mesh ou MQTT.
 * 
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#ifndef SECURE_PAYLOAD_H
#define SECURE_PAYLOAD_H

#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Tamanho da chave AES-256 (32 bytes / 256 bits). */
#define AES_KEY_SIZE_BYTES  32

/** @brief Tamanho do bloco do AES (16 bytes). */
#define AES_BLOCK_SIZE_BYTES 16

/**
 * @brief Inicializa o módulo criptográfico carregando a chave e vetor de inicialização (IV).
 * @details Configura a chave estática local de criptografia utilizada na cifragem do payload.
 * 
 * @param[in] key Ponteiro para array contendo a chave de 32 bytes.
 * 
 * @pre O hardware do ESP32 deve estar inicializado.
 * @post A chave é armazenada internamente na RAM protegida.
 * 
 * @return esp_err_t ESP_OK em caso de sucesso, ou erro do ESP-IDF.
 */
esp_err_t secure_payload_init(const uint8_t *key);

/**
 * @brief Criptografa uma string de texto em formato JSON usando AES-256-CBC com preenchimento PKCS#7.
 * @details Aloca ou usa buffers para retornar a string cifrada em codificação hexadecimal 
 *          para transmissão segura.
 * 
 * @param[in] input_str String C original terminada com '\0' contendo o JSON.
 * @param[out] output_hex Buffer de saída onde será gravado o resultado criptografado em hexadecimal.
 * @param[in] max_output_len Capacidade máxima em bytes do buffer de saída.
 * 
 * @pre O módulo deve ter sido iniciado com secure_payload_init.
 * @post O output_hex conterá a representação hex da mensagem criptografada.
 * 
 * @return esp_err_t ESP_OK em caso de sucesso.
 *                   ESP_ERR_INVALID_ARG se os ponteiros forem nulos.
 *                   ESP_ERR_NO_MEM se o buffer de saída for muito pequeno.
 */
esp_err_t secure_payload_encrypt(const char *input_str, char *output_hex, size_t max_output_len);

#ifdef __cplusplus
}
#endif

#endif /* SECURE_PAYLOAD_H */
