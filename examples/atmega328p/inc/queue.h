#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stddef.h>
#include <mutex.h>
#include <semaphore.h>

typedef struct {
  void** buf;
  uint32_t capacity;
  uint32_t last_element;
  uint32_t first_element;
  uint32_t size;
  OS_semaphore_handle consumer_sem;
  OS_semaphore_handle producer_sem;
  OS_mutex_handle queue_mutate_mutex;
} OS_queue_handle;

OS_queue_handle OS_queue_create(void** buf, uint32_t capacity);
uint8_t OS_queue_post(OS_queue_handle* qh, void* e);
void* OS_queue_pend(OS_queue_handle* qh);

#endif
