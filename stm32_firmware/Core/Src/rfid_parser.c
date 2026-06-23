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
