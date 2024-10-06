#include "semaphore.h"
#include <queue.h>


OS_queue_handle OS_create_queue(void** buf, uint16_t capacity) {
  OS_queue_handle qh = {
    .buf = buf,
    .capacity = capacity,
    .last_element = 0,
    .first_element = 0,
    .size = 0,
    .producer_sem = OS_create_semaphore(capacity),
    .consumer_sem = OS_create_semaphore(0),
    .queue_mutate_mutex = OS_create_mutex(),
  };

  return qh;
}


uint8_t OS_queue_post(OS_queue_handle* qh, void* e) {
  OS_sem_wait(&qh->producer_sem);

  OS_lock(&qh->queue_mutate_mutex);
  qh->buf[qh->last_element] = e;
  qh->last_element = (++qh->last_element)%qh->capacity;
  qh->size++;
  OS_unlock(&qh->queue_mutate_mutex);

  OS_sem_signal(&qh->consumer_sem);

  return 0;
}
void* OS_queue_pend(OS_queue_handle* qh) {
  OS_sem_wait(&qh->consumer_sem);

  OS_lock(&qh->queue_mutate_mutex);
  void* e = qh->buf[qh->first_element];
  qh->first_element = (++qh->first_element)%qh->capacity;
  qh->size--;
  OS_unlock(&qh->queue_mutate_mutex);

  OS_sem_signal(&qh->producer_sem);

  return e;
}
