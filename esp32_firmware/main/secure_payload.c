/**
 * @file secure_payload.c
 * @brief Implementacao do modulo de seguranca e criptografia de payloads.
 * @details Este modulo implementa a criptografia simetrica AES-256-CBC
 *          com preenchimento PKCS#7 utilizando a biblioteca nativa mbedTLS do ESP-IDF.
 * 
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#include "secure_payload.h"
#include <string.h>
#include <stdbool.h>
#include "mbedtls/aes.h"
#include "esp_log.h"
#include "esp_random.h"

static const char *TAG = "SECURE_PAYLOAD";

/** @brief Chave de criptografia local em RAM protegida. */
static uint8_t local_aes_key[AES_KEY_SIZE_BYTES];

/** @brief Flag indicando se o modulo foi devidamente configurado. */
static bool is_initialized = false;

esp_err_t secure_payload_init(const uint8_t *key)
{
    if (key == NULL) {
        ESP_LOGE(TAG, "Chave nula passada na inicializacao.");
        return ESP_ERR_INVALID_ARG;
    }
    
    memcpy(local_aes_key, key, AES_KEY_SIZE_BYTES);
    is_initialized = true;
    
    ESP_LOGI(TAG, "Modulo criptografico AES-256 inicializado com sucesso.");
    return ESP_OK;
}

esp_err_t secure_payload_encrypt(const char *input_str, char *output_hex, size_t max_output_len)
{
    if (!is_initialized) {
        ESP_LOGE(TAG, "Erro: Tentativa de criptografar sem iniciar o modulo.");
        return ESP_ERR_INVALID_STATE;
    }
    
    if (input_str == NULL || output_hex == NULL) {
        ESP_LOGE(TAG, "Ponteiros de entrada/saida nulos.");
        return ESP_ERR_INVALID_ARG;
    }
    
    size_t input_len = strlen(input_str);
    
    // 1. Calcula o padding necessario no padrao PKCS#7
    size_t pad_len = AES_BLOCK_SIZE_BYTES - (input_len % AES_BLOCK_SIZE_BYTES);
    size_t padded_len = input_len + pad_len;
    
    // 2. Garante que o buffer de saida comporta IV (32 hex chars) + ciphertext hex + terminador
    size_t required_hex_len = (AES_BLOCK_SIZE_BYTES * 2) + (padded_len * 2) + 1;
    if (max_output_len < required_hex_len) {
        ESP_LOGE(TAG, "Buffer de saida muito pequeno. Requerido: %d, Disponivel: %d", 
                 (int)required_hex_len, (int)max_output_len);
        return ESP_ERR_NO_MEM;
    }
    
    // 3. Gera IV aleatorio de 16 bytes via hardware RNG do ESP32
    uint8_t iv[AES_BLOCK_SIZE_BYTES];
    esp_fill_random(iv, AES_BLOCK_SIZE_BYTES);
    
    // 4. Aloca memoria para os dados com padding
    uint8_t *padded_data = (uint8_t *)malloc(padded_len);
    if (padded_data == NULL) {
        ESP_LOGE(TAG, "Falha de alocacao temporaria de heap.");
        return ESP_ERR_NO_MEM;
    }
    
    // Prepara a mensagem com padding PKCS#7
    memcpy(padded_data, input_str, input_len);
    for (size_t i = input_len; i < padded_len; i++) {
        padded_data[i] = (uint8_t)pad_len;
    }
    
    // 5. Prepara buffer para o resultado criptografado bruto
    uint8_t *encrypted_data = (uint8_t *)malloc(padded_len);
    if (encrypted_data == NULL) {
        free(padded_data);
        ESP_LOGE(TAG, "Falha de alocacao de buffer criptografado.");
        return ESP_ERR_NO_MEM;
    }
    
    // 6. Executa a criptografia AES-256-CBC via MbedTLS
    mbedtls_aes_context aes_ctx;
    mbedtls_aes_init(&aes_ctx);
    
    int ret = mbedtls_aes_setkey_enc(&aes_ctx, local_aes_key, 256);
    if (ret != 0) {
        ESP_LOGE(TAG, "Erro ao configurar a chave AES: %d", ret);
        mbedtls_aes_free(&aes_ctx);
        free(padded_data);
        free(encrypted_data);
        return ESP_FAIL;
    }
    
    // Duplica o IV pois o mbedtls_aes_crypt_cbc atualiza o vetor in-place
    uint8_t temp_iv[AES_BLOCK_SIZE_BYTES];
    memcpy(temp_iv, iv, AES_BLOCK_SIZE_BYTES);
    
    ret = mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_ENCRYPT, padded_len, temp_iv, padded_data, encrypted_data);
    if (ret != 0) {
        ESP_LOGE(TAG, "Erro na cifragem do bloco: %d", ret);
        mbedtls_aes_free(&aes_ctx);
        free(padded_data);
        free(encrypted_data);
        return ESP_FAIL;
    }
    
    mbedtls_aes_free(&aes_ctx);
    free(padded_data);
    
    // 7. Prefixa o IV (16 bytes) ao ciphertext no output hex
    // Formato: hex(IV) + hex(ciphertext)
    for (size_t i = 0; i < AES_BLOCK_SIZE_BYTES; i++) {
        sprintf(output_hex + (i * 2), "%02X", iv[i]);
    }
    for (size_t i = 0; i < padded_len; i++) {
        sprintf(output_hex + (AES_BLOCK_SIZE_BYTES * 2) + (i * 2), "%02X", encrypted_data[i]);
    }
    output_hex[(AES_BLOCK_SIZE_BYTES * 2) + (padded_len * 2)] = '\0';
    
    free(encrypted_data);
    return ESP_OK;
}
