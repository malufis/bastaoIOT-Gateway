#ifndef RFID_PARSER_H
#define RFID_PARSER_H

#include <stdint.h>

#define RFID_BUFFER_SIZE 512

typedef struct {
    uint8_t raw_data[RFID_BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
} RFID_Buffer_t;

void RFID_Init(void);
void RFID_StoreByte(uint8_t byte, uint8_t is_yrm100);
uint8_t RFID_HasData(uint8_t is_yrm100);

void RFID_Process_YRM100(void);
void RFID_Process_WL134(void);
void YRM100_SetTXPower(uint8_t dbm);

#endif
