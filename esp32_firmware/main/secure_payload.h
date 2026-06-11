/**
 * @file secure_payload.h
 * @brief Cabecalho do modulo de seguranca e criptografia de payloads.
 * @details Este modulo define as rotinas de criptografia simetrica AES para
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
 * @brief Inicializa o modulo criptografico carregando a chave e vetor de inicializacao (IV).
 * @details Configura a chave estatica local de criptografia utilizada na cifragem do payload.
 * 
 * @param[in] key Ponteiro para array contendo a chave de 32 bytes.
 * 
 * @pre O hardware do ESP32 deve estar inicializado.
 * @post A chave e armazenada internamente na RAM protegida.
 * 
 * @return esp_err_t ESP_OK em caso de sucesso, ou erro do ESP-IDF.
 */
esp_err_t secure_payload_init(const uint8_t *key);

/**
 * @brief Criptografa uma string de texto em formato JSON usando AES-256-CBC com preenchimento PKCS#7.
 * @details Gera um IV aleatorio de 16 bytes (via esp_fill_random) e o prefixa ao
 *          ciphertext no buffer de saida. Formato: hex(IV 16B) + hex(ciphertext).
 * 
 * @param[in] input_str String C original terminada com '\0' contendo o JSON.
 * @param[out] output_hex Buffer de saida onde sera gravado IV + ciphertext em hexadecimal.
 *                        Tamanho necessario: (16 + padded_len) * 2 + 1 bytes.
 * @param[in] max_output_len Capacidade maxima em bytes do buffer de saida.
 * 
 * @pre O modulo deve ter sido iniciado com secure_payload_init.
 * @post O output_hex contera 32 chars de IV hex + N chars de ciphertext hex.
 * 
 * @return esp_err_t ESP_OK em caso de sucesso.
 *                   ESP_ERR_INVALID_ARG se os ponteiros forem nulos.
 *                   ESP_ERR_NO_MEM se o buffer de saida for muito pequeno.
 */
esp_err_t secure_payload_encrypt(const char *input_str, char *output_hex, size_t max_output_len);

#ifdef __cplusplus
}
#endif

#endif /* SECURE_PAYLOAD_H */
