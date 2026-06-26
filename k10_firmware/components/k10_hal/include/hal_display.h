#ifndef HAL_DISPLAY_H
#define HAL_DISPLAY_H

#include "esp_lcd_types.h"

/**
 * @brief Inicializa o display LCD e o Touch (SST7789 e I2C Touch).
 *        Prepara a camada inferior para conectar com a LVGL.
 */
void hal_display_init(void);

/**
 * @brief Retorna o handle do painel LCD configurado.
 */
esp_lcd_panel_handle_t hal_display_get_panel_handle(void);

/**
 * @brief Retorna o handle do barramento de IO (SPI/DMA) para sincronismo de V-Sync.
 */
esp_lcd_panel_io_handle_t hal_display_get_io_handle(void);

/**
 * @brief Controla o estado do backlight do display LCD.
 * @param on true para ligar, false para desligar.
 */
void hal_display_set_backlight(bool on);

/**
 * @brief Reseta o temporizador de inatividade e liga o backlight se estiver desligado.
 */
void hal_display_reset_inactivity(void);

/**
 * @brief Retorna se o backlight está atualmente ligado.
 */
bool hal_display_is_backlight_on(void);

/**
 * @brief Verifica se o tempo de inatividade expirou (60s) e desliga o backlight.
 */
void hal_display_check_timeout(void);

#endif
