#ifndef CONTEST_RING_BUFFER_H
#define CONTEST_RING_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *data;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t size;
} contest_ring_buffer_t;

bool contest_ring_buffer_init(contest_ring_buffer_t *buffer, uint8_t *storage, size_t capacity);
void contest_ring_buffer_reset(contest_ring_buffer_t *buffer);
bool contest_ring_buffer_push(contest_ring_buffer_t *buffer, uint8_t byte);
bool contest_ring_buffer_pop(contest_ring_buffer_t *buffer, uint8_t *byte_out);
bool contest_ring_buffer_peek(const contest_ring_buffer_t *buffer, size_t offset, uint8_t *byte_out);
size_t contest_ring_buffer_size(const contest_ring_buffer_t *buffer);
size_t contest_ring_buffer_free(const contest_ring_buffer_t *buffer);

#endif
