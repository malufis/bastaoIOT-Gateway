#include "hal_buzzer.h"
#include "esp_log.h"
#include "driver/i2s_std.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <math.h>

static const char *TAG = "HAL_BUZZER";

/* ── Pinagem I2S K10 (Unihiker) — Speaker NS4168 ─────── */
#define I2S_BCLK_GPIO       0
#define I2S_LRCK_GPIO       38
#define I2S_SDI_GPIO        45   /* Dado TX do ESP32 → SDI do NS4168 */

/* ── Config I2S ───────────────────────────────────────── */
#define I2S_SAMPLE_RATE     44100

/* ── Sine wave ────────────────────────────────────────── */
#define SINE_TABLE_SIZE     256
#define SINE_AMPLITUDE      16000   /* ~75% de 32767 */

static int16_t s_sine_table[SINE_TABLE_SIZE];
static i2s_chan_handle_t s_tx_handle = NULL;

#define BEEP_FREQ_HZ        1000
#define ALERT_FREQ_HZ       2000

static bool s_initialized = false;
static volatile bool s_playing = false;

/* ── Tabela seno ──────────────────────────────────────── */
static void generate_sine_table(void) {
    for (int i = 0; i < SINE_TABLE_SIZE; i++) {
        float angle = (2.0f * M_PI * i) / SINE_TABLE_SIZE;
        s_sine_table[i] = (int16_t)(sinf(angle) * SINE_AMPLITUDE);
    }
}

/* ── Toca um tom de forma síncrona (bloqueia a task chamadora) ── */
static esp_err_t play_tone(uint32_t freq_hz, uint32_t duration_ms) {
    if (!s_tx_handle) return ESP_ERR_INVALID_STATE;

    /* Habilita canal I2S apenas durante a reprodução */
    esp_err_t err = i2s_channel_enable(s_tx_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "i2s_channel_enable falhou: %s", esp_err_to_name(err));
        return err;
    }

    uint32_t total_samples = (I2S_SAMPLE_RATE * duration_ms) / 1000;
    uint32_t phase_step = (freq_hz * SINE_TABLE_SIZE) / I2S_SAMPLE_RATE;
    uint32_t phase = 0;

    int16_t buf[128 * 2];  /* stereo */

    while (total_samples > 0) {
        size_t to_send = (total_samples > 128) ? 128 : total_samples;
        for (size_t i = 0; i < to_send; i++) {
            int16_t sample = s_sine_table[phase % SINE_TABLE_SIZE];
            buf[i * 2]     = sample;
            buf[i * 2 + 1] = sample;
            phase += phase_step;
        }
        size_t bw = 0;
        i2s_channel_write(s_tx_handle, buf, to_send * 2 * sizeof(int16_t),
                          &bw, pdMS_TO_TICKS(200));
        total_samples -= to_send;
    }

    /* Envia silêncio para limpar o pipeline interno do NS4168 */
    memset(buf, 0, sizeof(buf));
    for (int i = 0; i < 8; i++) {  /* ~5ms de silêncio em blocos de 128 samples */
        size_t bw = 0;
        i2s_channel_write(s_tx_handle, buf, sizeof(buf), &bw, pdMS_TO_TICKS(50));
    }

    /* DESABILITA o canal — para BCLK/LRCK imediatamente */
    i2s_channel_disable(s_tx_handle);

    return ESP_OK;
}

/* ── Task de beep (mantém API non-blocking) ────────────── */
typedef struct {
    uint32_t freq_hz;
    uint32_t duration_ms;
    uint32_t pause_ms;
    uint8_t  repeats;
} beep_params_t;

static beep_params_t s_params;

static void beep_task(void *arg) {
    const beep_params_t *p = &s_params;

    ESP_LOGI(TAG, "beep: freq=%luHz dur=%lums rep=%d",
             p->freq_hz, p->duration_ms, p->repeats);

    uint8_t max_rep = (p->repeats == 0) ? 255 : p->repeats;

    for (uint8_t rep = 0; rep < max_rep; rep++) {
        play_tone(p->freq_hz, p->duration_ms);

        /* Pausa entre beeps */
        if (p->pause_ms > 0 && rep < max_rep - 1) {
            vTaskDelay(pdMS_TO_TICKS(p->pause_ms));
        }
    }

    s_playing = false;
    ESP_LOGI(TAG, "beep finalizado");
    vTaskDelete(NULL);
}

static esp_err_t start_beep(uint32_t freq, uint32_t duration, uint32_t pause, uint8_t repeats) {
    if (s_playing) {
        ESP_LOGW(TAG, "beep ignorado: ja tocando");
        return ESP_ERR_INVALID_STATE;
    }
    if (!s_initialized) {
        ESP_LOGW(TAG, "beep ignorado: I2S nao inicializado");
        return ESP_ERR_INVALID_STATE;
    }

    s_params.freq_hz = freq;
    s_params.duration_ms = duration;
    s_params.pause_ms = pause;
    s_params.repeats = repeats;
    s_playing = true;

    BaseType_t ret = xTaskCreatePinnedToCore(beep_task, "beep", 4096, NULL, 3, NULL, 1);
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Falha ao criar beep_task");
        s_playing = false;
        return ESP_FAIL;
    }
    return ESP_OK;
}

/* ── API publica ──────────────────────────────────────── */

esp_err_t hal_buzzer_init(void) {
    ESP_LOGI(TAG, "Inicializando I2S para speaker NS4168 (BCLK=%d LRCK=%d SDI=%d)",
             I2S_BCLK_GPIO, I2S_LRCK_GPIO, I2S_SDI_GPIO);

    /* 1. Gera tabela seno */
    generate_sine_table();

    /* 2. Configura canal I2S TX (DMA mínimo) */
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    chan_cfg.dma_desc_num = 2;
    chan_cfg.dma_frame_num = 240;

    esp_err_t err = i2s_new_channel(&chan_cfg, &s_tx_handle, NULL);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "i2s_new_channel falhou: %s", esp_err_to_name(err));
        return err;
    }

    /* 3. Configura I2S Standard Mode */
    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(I2S_SAMPLE_RATE),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = I2S_BCLK_GPIO,
            .ws   = I2S_LRCK_GPIO,
            .dout = I2S_SDI_GPIO,
            .din  = I2S_GPIO_UNUSED,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
            },
        },
    };

    err = i2s_channel_init_std_mode(s_tx_handle, &std_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "i2s_channel_init_std_mode falhou: %s", esp_err_to_name(err));
        i2s_del_channel(s_tx_handle);
        s_tx_handle = NULL;
        return err;
    }

    /* IMPORTANTE: NÃO habilita o canal aqui.
     * O canal fica DESABILITADO entre beeps.
     * play_tone() habilita antes de tocar e desabilita depois. */

    s_initialized = true;
    ESP_LOGI(TAG, "I2S speaker inicializado com sucesso (canal desabilitado entre beeps)");
    return ESP_OK;
}

esp_err_t hal_buzzer_beep_short(void) {
    return start_beep(BEEP_FREQ_HZ, 100, 0, 1);
}

esp_err_t hal_buzzer_beep_long(void) {
    return start_beep(BEEP_FREQ_HZ, 300, 0, 1);
}

esp_err_t hal_buzzer_beep_double(void) {
    return start_beep(BEEP_FREQ_HZ, 100, 100, 2);
}

esp_err_t hal_buzzer_beep_alert(void) {
    return start_beep(ALERT_FREQ_HZ, 500, 0, 1);
}

void hal_buzzer_stop(void) {
    if (s_playing && s_tx_handle) {
        i2s_channel_disable(s_tx_handle);
    }
    s_playing = false;
}

bool hal_buzzer_is_playing(void) {
    return s_playing;
}
