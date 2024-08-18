#ifndef DEERTOS_H
#define DEERTOS_H

#include <stdint.h>

typedef struct {
  void* sp;
  uint32_t timeout;
  uint8_t priority;
} OSThread;

typedef void(*OSThreadHandler)();

int OS_create_thread(OSThread* me, uint8_t priority, OSThreadHandler threadHandler, void* stkSto, uint32_t stkSize);
int OS_init();
void OS_start();
void OS_delay(uint32_t miliseconds);

#endif
