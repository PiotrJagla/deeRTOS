#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>

typedef struct {
  int32_t val;
} OS_semaphore_handle;

OS_semaphore_handle OS_create_semaphore(int32_t init_val);
void OS_sem_wait(OS_semaphore_handle* sh);
void OS_sem_signal(OS_semaphore_handle* sh);

#endif
