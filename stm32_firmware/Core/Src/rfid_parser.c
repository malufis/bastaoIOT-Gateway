#include "rfid_parser.h"
#include <string.h>
#include <stdio.h>

#include "main.h"

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;

static RFID_Buffer_t buffer_yrm100;
static RFID_Buffer_t buffer_wl134;

static uint16_t yrm100_overflow_count = 0;
static uint16_t wl134_overflow_count = 0;

static void reverse_str(char *str, int len)
{
    int i = 0;
    int j = len - 1;
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

static void format_uint64_padded(char *buf, uint64_t val, int pad_len)
{
    buf[pad_len] = '\0';
    for (int i = pad_len - 1; i >= 0; i--) {
        buf[i] = '0' + (val % 10);
        val /= 10;
    }
}

static uint64_t hex_to_uint64(const char *hex_str)
{
    uint64_t val = 0;
    while (*hex_str) {
        uint8_t byte = (uint8_t)*hex_str++;
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <= 'f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <= 'F') byte = byte - 'A' + 10;
        else continue;
        val = (val << 4) | byte;
    }
    return val;
}

void RFID_Init(void)
{
    memset(&buffer_yrm100, 0, sizeof(RFID_Buffer_t));
    memset(&buffer_wl134, 0, sizeof(RFID_Buffer_t));
    yrm100_overflow_count = 0;
    wl134_overflow_count = 0;
}

void RFID_StoreByte(uint8_t byte, uint8_t is_yrm100)
{
    RFID_Buffer_t *buf = is_yrm100 ? &buffer_yrm100 : &buffer_wl134;
    uint16_t next_head = (buf->head + 1) % RFID_BUFFER_SIZE;
    if (next_head == buf->tail) {
        if (is_yrm100)
            yrm100_overflow_count++;
        else
            wl134_overflow_count++;
        return;
    }
    buf->raw_data[buf->head] = byte;
    buf->head = next_head;
}

uint8_t RFID_HasData(uint8_t is_yrm100)
{
    RFID_Buffer_t *buf = is_yrm100 ? &buffer_yrm100 : &buffer_wl134;
    return buf->head != buf->tail;
}

void RFID_Process_YRM100(void)
{
    while (buffer_yrm100.head != buffer_yrm100.tail) {
        if (buffer_yrm100.raw_data[buffer_yrm100.tail] == 0xBB) {
            uint16_t available = (buffer_yrm100.head + RFID_BUFFER_SIZE - buffer_yrm100.tail) % RFID_BUFFER_SIZE;
            if (available >= 7) {
                uint16_t pl = (buffer_yrm100.raw_data[(buffer_yrm100.tail + 3) % RFID_BUFFER_SIZE] << 8) |
                              buffer_yrm100.raw_data[(buffer_yrm100.tail + 4) % RFID_BUFFER_SIZE];

                if (available >= (7 + pl)) {
                    uint8_t checksum = 0;
                    for (int i = 1; i < 5 + pl; i++) {
                        checksum += buffer_yrm100.raw_data[(buffer_yrm100.tail + i) % RFID_BUFFER_SIZE];
                    }

                    if (checksum == buffer_yrm100.raw_data[(buffer_yrm100.tail + 5 + pl) % RFID_BUFFER_SIZE] &&
                        buffer_yrm100.raw_data[(buffer_yrm100.tail + 6 + pl) % RFID_BUFFER_SIZE] == 0x7E) {

                        if (buffer_yrm100.raw_data[(buffer_yrm100.tail + 2) % RFID_BUFFER_SIZE] == 0x22) {
                            char json[256];
                            char epc_hex[64] = {0};
                            uint16_t epc_len = pl - 5;

                            for (int i = 0; i < epc_len; i++) {
                                sprintf(epc_hex + strlen(epc_hex), "%02X",
                                        buffer_yrm100.raw_data[(buffer_yrm100.tail + 8 + i) % RFID_BUFFER_SIZE]);
                            }
                            sprintf(json, "{\"type\":\"rfid\",\"model\":\"YRM100\",\"tag\":\"%s\"}\n", epc_hex);
                            HAL_UART_Transmit(&huart2, (uint8_t *)json, strlen(json), 100);
                        }

                        buffer_yrm100.tail = (buffer_yrm100.tail + 7 + pl) % RFID_BUFFER_SIZE;
                        continue;
                    }
                } else {
                    return;
                }
            } else {
                return;
            }
        }
        buffer_yrm100.tail = (buffer_yrm100.tail + 1) % RFID_BUFFER_SIZE;
    }
}

void RFID_Process_WL134(void)
{
    while (buffer_wl134.head != buffer_wl134.tail) {
        if (buffer_wl134.raw_data[buffer_wl134.tail] == 0x02) {
            uint16_t available = (buffer_wl134.head + RFID_BUFFER_SIZE - buffer_wl134.tail) % RFID_BUFFER_SIZE;
            if (available >= 30) {
                if (buffer_wl134.raw_data[(buffer_wl134.tail + 29) % RFID_BUFFER_SIZE] == 0x03) {
                    uint8_t checksum = 0;
                    for (int i = 1; i <= 26; i++) {
                        checksum ^= buffer_wl134.raw_data[(buffer_wl134.tail + i) % RFID_BUFFER_SIZE];
                    }
                    uint8_t expected_checksum = buffer_wl134.raw_data[(buffer_wl134.tail + 27) % RFID_BUFFER_SIZE];
                    uint8_t expected_inverted = buffer_wl134.raw_data[(buffer_wl134.tail + 28) % RFID_BUFFER_SIZE];

                    if (checksum == expected_checksum && (uint8_t)(~checksum) == expected_inverted) {
                        char card_hex[11] = {0};
                        for (int i = 0; i < 10; i++) {
                            card_hex[i] = buffer_wl134.raw_data[(buffer_wl134.tail + 1 + i) % RFID_BUFFER_SIZE];
                        }
                        char country_hex[5] = {0};
                        for (int i = 0; i < 4; i++) {
                            country_hex[i] = buffer_wl134.raw_data[(buffer_wl134.tail + 11 + i) % RFID_BUFFER_SIZE];
                        }

                        reverse_str(card_hex, 10);
                        reverse_str(country_hex, 4);

                        uint64_t card_dec = hex_to_uint64(card_hex);
                        uint32_t country_dec = (uint32_t)hex_to_uint64(country_hex);

                        char card_dec_str[24];
                        format_uint64_padded(card_dec_str, card_dec, 12);

                        char json[128];
                        sprintf(json, "{\"type\":\"rfid\",\"model\":\"WL134\",\"tag\":\"%03lu%s\"}\n",
                                (unsigned long)country_dec, card_dec_str);
                        HAL_UART_Transmit(&huart2, (uint8_t *)json, strlen(json), 100);

                        buffer_wl134.tail = (buffer_wl134.tail + 30) % RFID_BUFFER_SIZE;
                        continue;
                    }
                }
            } else {
                return;
            }
        }
        buffer_wl134.tail = (buffer_wl134.tail + 1) % RFID_BUFFER_SIZE;
    }
}

uint16_t RFID_GetOverflowCount(uint8_t is_yrm100)
{
    return is_yrm100 ? yrm100_overflow_count : wl134_overflow_count;
}

void RFID_ResetOverflowCount(uint8_t is_yrm100)
{
    if (is_yrm100)
        yrm100_overflow_count = 0;
    else
        wl134_overflow_count = 0;
}

/* ---------------------------------------------------------------------------
 * Comandos de configuracao do modulo YRM100
 * --------------------------------------------------------------------------- */

/**
 * @brief Aguarda ate N bytes no buffer YRM100 ou timeout de 200ms.
 * @return 1 se dados chegaram, 0 se timeout.
 */
static uint8_t wait_yrm100_response(void)
{
    uint32_t start = HAL_GetTick();
    while ((HAL_GetTick() - start) < 200) {
        if (RFID_HasData(1)) return 1;
    }
    return 0;
}

void YRM100_SetMode(void)
{
    uint8_t cmd[] = {
        0xBB, 0x00, 0xF5, 0x00, 0x01,
        0x01,       // Mode parameter
        0x00,       // CS
        0x7E
    };
    cmd[6] = 0x00 + 0xF5 + 0x00 + 0x01 + 0x01;

    HAL_UART_Transmit(&huart4, cmd, sizeof(cmd), 100);
    wait_yrm100_response();
    RFID_Process_YRM100();
}

void YRM100_SaveConfig(void)
{
    uint8_t cmd[] = {
        0xBB, 0x00, 0x09, 0x00, 0x01,
        0x01,       // Save parameter
        0x00,       // CS
        0x7E
    };
    cmd[6] = 0x00 + 0x09 + 0x00 + 0x01 + 0x01;

    HAL_UART_Transmit(&huart4, cmd, sizeof(cmd), 100);
    wait_yrm100_response();
    RFID_Process_YRM100();
}

void YRM100_SetRegion(uint8_t region)
{
    uint8_t cmd[] = {
        0xBB,       // Header
        0x00,       // Type (Command)
        0x07,       // Cmd (Set Region)
        0x00, 0x01, // PL = 1 byte
        region,     // Param: regiao
        0x00,       // CS (preenchido abaixo)
        0x7E        // End
    };
    // Calcula checksum: Type + Cmd + PL + Param
    cmd[6] = 0x00 + 0x07 + 0x00 + 0x01 + region;

    HAL_UART_Transmit(&huart4, cmd, sizeof(cmd), 100);
    wait_yrm100_response();
    // Processa frame de resposta para limpar buffer
    RFID_Process_YRM100();
}

void YRM100_SetTXPower(uint8_t dbm)
{
    // Converte dBm para centesimos de dBm (ex: 20 dBm -> 2000 -> 0x07D0)
    uint16_t power_cdBm = (uint16_t)dbm * 100;
    uint8_t power_h = (power_cdBm >> 8) & 0xFF;
    uint8_t power_l = power_cdBm & 0xFF;

    uint8_t cmd[] = {
        0xBB,       // Header
        0x00,       // Type (Command)
        0xB6,       // Cmd (Set TX Power)
        0x00, 0x02, // PL = 2 bytes
        power_h,    // Potencia MSB
        power_l,    // Potencia LSB
        0x00,       // CS (preenchido abaixo)
        0x7E        // End
    };
    // Calcula checksum: Type + Cmd + PL + Param(2)
    cmd[7] = (uint8_t)(0x00 + 0xB6 + 0x00 + 0x02 + power_h + power_l);

    HAL_UART_Transmit(&huart4, cmd, sizeof(cmd), 100);
    wait_yrm100_response();
    RFID_Process_YRM100();
}

void YRM100_StartContinuousRead(void)
{
    // Frame: BB 00 27 00 03 22 FF FF <CS> 7E
    // Reserved=0x22, CNT=0xFFFF (65535 ciclos)
    uint8_t cmd[] = {
        0xBB, 0x00, 0x27, 0x00, 0x03,
        0x22,       // Reserved
        0xFF, 0xFF, // CNT = 65535 ciclos
        0x00,       // CS (preenchido abaixo)
        0x7E
    };
    // Checksum: Type + Cmd + PL + Param(3)
    cmd[9] = (uint8_t)(0x00 + 0x27 + 0x00 + 0x03 + 0x22 + 0xFF + 0xFF);

    HAL_UART_Transmit(&huart4, cmd, sizeof(cmd), 100);
    wait_yrm100_response();
    RFID_Process_YRM100();
}

void YRM100_StopContinuousRead(void)
{
    uint8_t cmd[] = {0xBB, 0x00, 0x28, 0x00, 0x00, 0x28, 0x7E};

    HAL_UART_Transmit(&huart4, cmd, sizeof(cmd), 100);
    wait_yrm100_response();
    RFID_Process_YRM100();
}

/* ---------------------------------------------------------------------------
 * Comandos de leitura de configuracao do YRM100
 * --------------------------------------------------------------------------- */

/**
 * @brief Aguarda resposta do YRM100 e extrai um frame bruto do buffer circular.
 *
 * Varre o buffer YRM100 em busca de um frame valido (0xBB ... 0x7E com checksum).
 * Copia o frame completo para buf e avanca tail.
 *
 * @param buf     Buffer de saida para o frame bruto
 * @param len     Ponteiro para receber o tamanho do frame copiado
 * @param max_len Tamanho maximo do buffer de saida
 * @return 1 se um frame foi extraido, 0 se timeout ou buffer vazio
 */
static uint8_t yrm100_read_raw_frame(uint8_t *buf, uint16_t *len, uint16_t max_len)
{
    if (!wait_yrm100_response()) return 0;

    while (buffer_yrm100.head != buffer_yrm100.tail) {
        if (buffer_yrm100.raw_data[buffer_yrm100.tail] == 0xBB) {
            uint16_t available = (buffer_yrm100.head + RFID_BUFFER_SIZE - buffer_yrm100.tail) % RFID_BUFFER_SIZE;
            if (available < 7) return 0;

            uint16_t pl = (buffer_yrm100.raw_data[(buffer_yrm100.tail + 3) % RFID_BUFFER_SIZE] << 8) |
                           buffer_yrm100.raw_data[(buffer_yrm100.tail + 4) % RFID_BUFFER_SIZE];
            uint16_t frame_len = 7 + pl;

            if (available < frame_len) return 0;

            // Valida checksum
            uint8_t checksum = 0;
            for (int i = 1; i < 5 + pl; i++)
                checksum += buffer_yrm100.raw_data[(buffer_yrm100.tail + i) % RFID_BUFFER_SIZE];

            if (checksum != buffer_yrm100.raw_data[(buffer_yrm100.tail + 5 + pl) % RFID_BUFFER_SIZE] ||
                buffer_yrm100.raw_data[(buffer_yrm100.tail + 6 + pl) % RFID_BUFFER_SIZE] != 0x7E) {
                buffer_yrm100.tail = (buffer_yrm100.tail + 1) % RFID_BUFFER_SIZE;
                continue;
            }

            // Copia frame para buffer de saida
            if (frame_len > max_len) {
                buffer_yrm100.tail = (buffer_yrm100.tail + frame_len) % RFID_BUFFER_SIZE;
                return 0;
            }
            for (uint16_t i = 0; i < frame_len; i++)
                buf[i] = buffer_yrm100.raw_data[(buffer_yrm100.tail + i) % RFID_BUFFER_SIZE];
            *len = frame_len;
            buffer_yrm100.tail = (buffer_yrm100.tail + frame_len) % RFID_BUFFER_SIZE;
            return 1;
        }
        buffer_yrm100.tail = (buffer_yrm100.tail + 1) % RFID_BUFFER_SIZE;
    }
    return 0;
}

void YRM100_ReadFirmwareVersion(char *buf, uint8_t max_len)
{
    // Comando 0x26 com payload=0 (leitura)
    uint8_t cmd[] = {0xBB, 0x00, 0x26, 0x00, 0x00, 0x26, 0x7E};

    HAL_UART_Transmit(&huart4, cmd, sizeof(cmd), 100);

    uint8_t frame[64];
    uint16_t frame_len = 0;
    if (yrm100_read_raw_frame(frame, &frame_len, sizeof(frame)) && frame_len >= 7 && frame[2] == 0x26) {
        // Frame: BB 00 26 <pl_h> <pl_l> <version_string> <CS> 7E
        uint16_t pl = (frame[3] << 8) | frame[4];
        uint8_t copy_len = (pl < max_len - 1) ? pl : (max_len - 1);
        for (uint8_t i = 0; i < copy_len; i++)
            buf[i] = frame[5 + i];  // payload comeca no indice 5
        buf[copy_len] = '\0';
    } else {
        buf[0] = '\0';
    }
}

uint8_t YRM100_ReadRegion(void)
{
    // Comando 0x07 com payload=0 (leitura de regiao)
    uint8_t cmd[] = {0xBB, 0x00, 0x07, 0x00, 0x00, 0x07, 0x7E};

    HAL_UART_Transmit(&huart4, cmd, sizeof(cmd), 100);

    uint8_t frame[16];
    uint16_t frame_len = 0;
    if (yrm100_read_raw_frame(frame, &frame_len, sizeof(frame)) && frame_len >= 7 && frame[2] == 0x07) {
        // Frame: BB 00 07 00 01 <region> <CS> 7E
        return frame[5];  // region no indice 5
    }
    return 0xFF;  // erro
}

uint8_t YRM100_ReadTXPower(void)
{
    // Comando 0xB6 com payload=0 (leitura de potencia)
    uint8_t cmd[] = {0xBB, 0x00, 0xB6, 0x00, 0x00, 0xB6, 0x7E};

    HAL_UART_Transmit(&huart4, cmd, sizeof(cmd), 100);

    uint8_t frame[16];
    uint16_t frame_len = 0;
    if (yrm100_read_raw_frame(frame, &frame_len, sizeof(frame)) && frame_len >= 8 && frame[2] == 0xB6) {
        // Frame: BB 00 B6 00 02 <power_h> <power_l> <CS> 7E
        uint16_t power_cdBm = ((uint16_t)frame[5] << 8) | frame[6];
        return (uint8_t)(power_cdBm / 100);  // converte centesimos para dBm
    }
    return 0xFF;  // erro
}

void YRM100_ReportConfig(void)
{
    char fw[16] = {0};
    uint8_t region = 0xFF;
    uint8_t power = 0xFF;

    // Le todas as configuracoes
    YRM100_ReadFirmwareVersion(fw, sizeof(fw));
    HAL_Delay(10);
    region = YRM100_ReadRegion();
    HAL_Delay(10);
    power = YRM100_ReadTXPower();

    // Monta JSON e envia para ESP32
    char json[128];
    const char *region_str = "??";
    if (region == 0x01) region_str = "CN900";
    else if (region == 0x02) region_str = "US";
    else if (region == 0x03) region_str = "EU";
    else if (region == 0x04) region_str = "CN800";

    sprintf(json, "{\"type\":\"yrm100_cfg\",\"fw\":\"%s\",\"region\":%d,\"region_name\":\"%s\",\"power\":%d}\n",
            (fw[0] ? fw : "N/A"), region, region_str, power);

    HAL_UART_Transmit(&huart2, (uint8_t *)json, strlen(json), 100);
}

void YRM100_Init(void)
{
    // Limpa qualquer dado residual no buffer
    buffer_yrm100.head = 0;
    buffer_yrm100.tail = 0;

    // 1. Set Mode
    YRM100_SetMode();
    HAL_Delay(20);

    // 2. Configura regiao para US/America (902-928 MHz)
    YRM100_SetRegion(0x02);
    HAL_Delay(20);

    // 3. Configura potencia TX (20 dBm)
    YRM100_SetTXPower(20);
    HAL_Delay(20);

    // 4. Salva configuracao na flash do modulo
    YRM100_SaveConfig();
    HAL_Delay(20);

    // 5. Le configuracoes de VOLTA do modulo
    YRM100_ReportConfig();
    HAL_Delay(20);

    // 6. Single poll sera feito no loop principal (100ms)
}

void YRM100_SinglePoll(void)
{
    uint8_t cmd[] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};
    HAL_UART_Transmit(&huart4, cmd, sizeof(cmd), 100);
    // Resposta processada por RFID_Process_YRM100() no loop principal
}

/* ---------------------------------------------------------------------------
 * Funcoes auxiliares para teste/diagnostico
 * --------------------------------------------------------------------------- */

void YRM100_FlushBuffer(void)
{
    buffer_yrm100.head = 0;
    buffer_yrm100.tail = 0;
}

uint16_t YRM100_ReadRawBuffer(uint8_t *buf, uint16_t max_len)
{
    uint16_t count = 0;
    while (buffer_yrm100.head != buffer_yrm100.tail && count < max_len) {
        buf[count++] = buffer_yrm100.raw_data[buffer_yrm100.tail];
        buffer_yrm100.tail = (buffer_yrm100.tail + 1) % RFID_BUFFER_SIZE;
    }
    return count;
}
