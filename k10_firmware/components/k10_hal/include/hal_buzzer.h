#pragma once

#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Inicializa o driver I2S para o speaker da K10 (NS4168).
 *
 * Chamado UMA VEZ no boot antes de qualquer beep.
 * Configura I2S no modo standard, 44100Hz, 16-bit, mono.
 *
 * @return ESP_OK em caso de sucesso
 */
esp_err_t hal_buzzer_init(void);

/**
 * @brief Emite um beep curto (100ms, 1kHz).
 *
 * Non-blocking: retorna imediatamente, beep roda em background.
 * Se ja estiver emitindo, ignora (nao sobrepoe).
 *
 * @return ESP_OK em caso de sucesso, ESP_ERR_INVALID_STATE se ja tocando
 */
esp_err_t hal_buzzer_beep_short(void);

/**
 * @brief Emite um beep longo (300ms, 1kHz).
 *
 * Non-blocking: retorna imediatamente, beep roda em background.
 * Se ja estiver emitindo, ignora (nao sobrepoe).
 *
 * @return ESP_OK em caso de sucesso, ESP_ERR_INVALID_STATE se ja tocando
 */
esp_err_t hal_buzzer_beep_long(void);

/**
 * @brief Emite um beep duplo (dois beeps de 100ms com 100ms de pausa).
 *
 * Non-blocking: retorna imediatamente, beep roda em background.
 *
 * @return ESP_OK em caso de sucesso
 */
esp_err_t hal_buzzer_beep_double(void);

/**
 * @brief Emite um beep de alerta (500ms, 2kHz).
 *
 * Non-blocking: retorna imediatamente.
 *
 * @return ESP_OK em caso de sucesso
 */
esp_err_t hal_buzzer_beep_alert(void);

/**
 * @brief Para qualquer beep em andamento e desliga o speaker.
 */
void hal_buzzer_stop(void);

/**
 * @brief Verifica se o speaker esta tocando.
 *
 * @return true se emitindo som
 */
bool hal_buzzer_is_playing(void);

#ifdef __cplusplus
}
#endif
