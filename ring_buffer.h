#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "project-conf.h"

typedef struct {
  uint16_t buffer[NUM_MEASUREMENTS];
  uint8_t capacity;
  uint8_t head;
  uint8_t tail;
} ring_buffer_t;

void ring_buffer_init(ring_buffer_t *buffer);
bool ring_buffer_write(ring_buffer_t *buffer, uint16_t value);
bool ring_buffer_read(ring_buffer_t *buffer, uint16_t *value);
uint8_t ring_buffer_size(ring_buffer_t *buffer);
bool ring_buffer_empty(ring_buffer_t *buffer);
bool ring_buffer_full(ring_buffer_t *buffer);