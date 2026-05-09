#include "contest_ring_buffer.h"

bool contest_ring_buffer_init(contest_ring_buffer_t *buffer, uint8_t *storage, size_t capacity)
{
    if (buffer == 0 || storage == 0 || capacity == 0u) {
        return false;
    }

    buffer->data = storage;
    buffer->capacity = capacity;
    buffer->head = 0u;
    buffer->tail = 0u;
    buffer->size = 0u;
    return true;
}

void contest_ring_buffer_reset(contest_ring_buffer_t *buffer)
{
    if (buffer == 0) {
        return;
    }

    buffer->head = 0u;
    buffer->tail = 0u;
    buffer->size = 0u;
}

bool contest_ring_buffer_push(contest_ring_buffer_t *buffer, uint8_t byte)
{
    if (buffer == 0 || buffer->size >= buffer->capacity) {
        return false;
    }

    /* Write at head, then advance head modulo capacity. */
    buffer->data[buffer->head] = byte;
    buffer->head = (buffer->head + 1u) % buffer->capacity;
    buffer->size += 1u;
    return true;
}

bool contest_ring_buffer_pop(contest_ring_buffer_t *buffer, uint8_t *byte_out)
{
    if (buffer == 0 || byte_out == 0 || buffer->size == 0u) {
        return false;
    }

    /* Read from tail, then advance tail modulo capacity. */
    *byte_out = buffer->data[buffer->tail];
    buffer->tail = (buffer->tail + 1u) % buffer->capacity;
    buffer->size -= 1u;
    return true;
}

bool contest_ring_buffer_peek(const contest_ring_buffer_t *buffer, size_t offset, uint8_t *byte_out)
{
    size_t index;

    if (buffer == 0 || byte_out == 0 || offset >= buffer->size) {
        return false;
    }

    index = (buffer->tail + offset) % buffer->capacity;
    *byte_out = buffer->data[index];
    return true;
}

size_t contest_ring_buffer_size(const contest_ring_buffer_t *buffer)
{
    return buffer == 0 ? 0u : buffer->size;
}

size_t contest_ring_buffer_free(const contest_ring_buffer_t *buffer)
{
    return buffer == 0 ? 0u : (buffer->capacity - buffer->size);
}
