#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h>

typedef struct {
    uint8_t *buffer;
    uint16_t size;
    volatile uint16_t head;
    volatile uint16_t tail;
} CircularBuffer_t;

void CircularBuffer_Init(CircularBuffer_t *cb, uint8_t *buf, uint16_t size);
uint8_t CircularBuffer_Write(CircularBuffer_t *cb, uint8_t byte);
uint8_t CircularBuffer_Read(CircularBuffer_t *cb, uint8_t *byte);
uint16_t CircularBuffer_Available(CircularBuffer_t *cb);
uint8_t CircularBuffer_IsEmpty(CircularBuffer_t *cb);
uint8_t CircularBuffer_IsFull(CircularBuffer_t *cb);
void CircularBuffer_Flush(CircularBuffer_t *cb);
uint8_t CircularBuffer_Peek(CircularBuffer_t *cb, uint16_t offset, uint8_t *byte);

#endif
