#ifndef DEERTOS_H
#define DEERTOS_H

#include <stdint.h>

typedef struct {
  void* sp;
  uint32_t timeout;
} OSThread;

typedef void(*OSThreadHandler)();

int OSThreadStart(OSThread* me, OSThreadHandler threadHandler, void* stkSto, uint32_t stkSize);
int OSInit();
void OSSched();
void OS_start();
void OS_delay(uint32_t miliseconds);

#endif
