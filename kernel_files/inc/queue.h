#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stddef.h>
#include <mutex.h>
#include <semaphore.h>

typedef struct {
  void** buf;
  uint16_t capacity;
  uint16_t last_element;
  uint16_t first_element;
  uint16_t size;
  OS_semaphore_handle consumer_sem;
  OS_semaphore_handle producer_sem;
  OS_mutex_handle queue_mutate_mutex;
} OS_queue_handle;

OS_queue_handle OS_create_queue(void** buf, uint16_t capacity);
uint8_t OS_queue_post(OS_queue_handle* qh, void* e);
void* OS_queue_pend(OS_queue_handle* qh);

#endif
