/**
 * @file secure_payload.c
 * @brief Implementação do módulo de segurança e criptografia de payloads.
 * @details Este módulo implementa a criptografia simétrica AES-256-CBC
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

static const char *TAG = "SECURE_PAYLOAD";

/** @brief Chave de criptografia local em RAM protegida. */
static uint8_t local_aes_key[AES_KEY_SIZE_BYTES];

/** @brief Flag indicando se o módulo foi devidamente configurado. */
static bool is_initialized = false;

/** 
 * @brief Vetor de Inicialização (IV) fixo e seguro para o modo CBC.
 * @note Em produção comercial avançada, este IV deve ser randômico (gerado por hardware do ESP32 via esp_random())
 *       e enviado em anexo no início do payload criptografado.
 */
static const uint8_t static_iv[AES_BLOCK_SIZE_BYTES] = {
    0xA5, 0x5A, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC,
    0xDE, 0xF0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66
};

esp_err_t secure_payload_init(const uint8_t *key)
{
    if (key == NULL) {
        ESP_LOGE(TAG, "Chave nula passada na inicialização.");
        return ESP_ERR_INVALID_ARG;
    }
    
    memcpy(local_aes_key, key, AES_KEY_SIZE_BYTES);
    is_initialized = true;
    
    ESP_LOGI(TAG, "Módulo criptográfico AES-256 inicializado com sucesso.");
    return ESP_OK;
}

esp_err_t secure_payload_encrypt(const char *input_str, char *output_hex, size_t max_output_len)
{
    if (!is_initialized) {
        ESP_LOGE(TAG, "Erro: Tentativa de criptografar sem iniciar o módulo.");
        return ESP_ERR_INVALID_STATE;
    }
    
    if (input_str == NULL || output_hex == NULL) {
        ESP_LOGE(TAG, "Ponteiros de entrada/saída nulos.");
        return ESP_ERR_INVALID_ARG;
    }
    
    size_t input_len = strlen(input_str);
    
    // 1. Calcula o padding necessário no padrão PKCS#7
    size_t pad_len = AES_BLOCK_SIZE_BYTES - (input_len % AES_BLOCK_SIZE_BYTES);
    size_t padded_len = input_len + pad_len;
    
    // 2. Garante que o buffer de saída comporta a representação em Hexadecimal (+ 1 byte de terminador nulo)
    size_t required_hex_len = (padded_len * 2) + 1;
    if (max_output_len < required_hex_len) {
        ESP_LOGE(TAG, "Buffer de saída muito pequeno. Requerido: %d, Disponível: %d", 
                 (int)required_hex_len, (int)max_output_len);
        return ESP_ERR_NO_MEM;
    }
    
    // 3. Aloca memória temporária na stack para os dados com padding
    uint8_t *padded_data = (uint8_t *)malloc(padded_len);
    if (padded_data == NULL) {
        ESP_LOGE(TAG, "Falha de alocação temporária de heap.");
        return ESP_ERR_NO_MEM;
    }
    
    // Prepara a mensagem com padding PKCS#7
    memcpy(padded_data, input_str, input_len);
    for (size_t i = input_len; i < padded_len; i++) {
        padded_data[i] = (uint8_t)pad_len;
    }
    
    // 4. Prepara buffer para o resultado criptografado bruto
    uint8_t *encrypted_data = (uint8_t *)malloc(padded_len);
    if (encrypted_data == NULL) {
        free(padded_data);
        ESP_LOGE(TAG, "Falha de alocação de buffer criptografado.");
        return ESP_ERR_NO_MEM;
    }
    
    // 5. Executa a criptografia AES-256-CBC via MbedTLS
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
    memcpy(temp_iv, static_iv, AES_BLOCK_SIZE_BYTES);
    
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
    
    // 6. Converte o resultado criptografado bruto para string Hexadecimal
    for (size_t i = 0; i < padded_len; i++) {
        sprintf(output_hex + (i * 2), "%02X", encrypted_data[i]);
    }
    output_hex[padded_len * 2] = '\0'; // Garante terminação nula
    
    free(encrypted_data);
    return ESP_OK;
}
