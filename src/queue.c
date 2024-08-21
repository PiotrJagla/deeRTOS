#include "queue.h"



OS_queue_handle OS_queue_create(void** buf, uint32_t capacity) {
  OS_queue_handle qh = {
    .buf = buf,
    .capaity = capacity,
    .last_element = 0,
    .first_element = 0,
    .size = 0;
  };

  return qh;
}


uint8_t OS_queue_post(OS_queue_handle* qh, void* e) {
  if(qh->size == qh->capaity) {
    return 1;
  }
  qh->buf[qh->last_element++] = e;
  qh->last_element = qh->last_element%qh->size;
  qh->size++;
  return 0;
}
void* OS_queue_pend(OS_queue_handle* qh) {
  if(qh->size == 0) {
    return NULL;
  }
  void* e = qh->buf[qh->first_element++];
  qh->first_element = qh->first_element%qh->size;
  qh->size--;
}
