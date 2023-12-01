#include "ring_buffer.h"

ring_buffer_t buffer = {
  .buffer = {0},
  .capacity = NUM_MEASUREMENTS,
  .head = 0,
  .tail = 0,
};



bool ring_buffer_write(ring_buffer_t *buffer, uint16_t value) {
  if (ring_buffer_full(buffer)) {
    return false;
  }

  buffer->buffer[buffer->head] = value;
  buffer->head = (buffer->head + 1) % buffer->capacity;

  return true;
}

bool ring_buffer_read(ring_buffer_t *buffer, uint16_t *value) {
  if (ring_buffer_empty(buffer)) {
    return false;
  }

  *value = buffer->buffer[buffer->tail];
  buffer->tail = (buffer->tail + 1) % buffer->capacity;

  return true;
}

uint8_t ring_buffer_size(ring_buffer_t *buffer) {
  return (buffer->head - buffer->tail + buffer->capacity) % buffer->capacity;
}

bool ring_buffer_empty(ring_buffer_t *buffer) {
  return ring_buffer_size(buffer) == 0;
}

bool ring_buffer_full(ring_buffer_t *buffer) {
  return ring_buffer_size(buffer) == buffer->capacity - 1;
}