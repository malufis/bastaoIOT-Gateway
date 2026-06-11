#include "circular_buffer.h"

void CircularBuffer_Init(CircularBuffer_t *cb, uint8_t *buf, uint16_t size)
{
    cb->buffer = buf;
    cb->size = size;
    cb->head = 0;
    cb->tail = 0;
}

uint8_t CircularBuffer_Write(CircularBuffer_t *cb, uint8_t byte)
{
    uint16_t next = (cb->head + 1) % cb->size;
    if (next == cb->tail) {
        return 0;
    }
    cb->buffer[cb->head] = byte;
    cb->head = next;
    return 1;
}

uint8_t CircularBuffer_Read(CircularBuffer_t *cb, uint8_t *byte)
{
    if (cb->head == cb->tail) {
        return 0;
    }
    *byte = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % cb->size;
    return 1;
}

uint16_t CircularBuffer_Available(CircularBuffer_t *cb)
{
    return (cb->head + cb->size - cb->tail) % cb->size;
}

uint8_t CircularBuffer_IsEmpty(CircularBuffer_t *cb)
{
    return cb->head == cb->tail;
}

uint8_t CircularBuffer_IsFull(CircularBuffer_t *cb)
{
    return ((cb->head + 1) % cb->size) == cb->tail;
}

void CircularBuffer_Flush(CircularBuffer_t *cb)
{
    cb->head = 0;
    cb->tail = 0;
}

uint8_t CircularBuffer_Peek(CircularBuffer_t *cb, uint16_t offset, uint8_t *byte)
{
    if (offset >= CircularBuffer_Available(cb)) {
        return 0;
    }
    *byte = cb->buffer[(cb->tail + offset) % cb->size];
    return 1;
}
