#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
  void** buf;
  uint32_t capaity;
  uint32_t last_element;
  uint32_t first_element;
  uint32_t size;
} OS_queue_handle;

OS_queue_handle OS_queue_create(void** buf, uint32_t capacity);
void OS_queue_post(OS_queue_handle* qh, void* e);
void* OS_queue_pend(OS_queue_handle* qh);

#endif
