#ifndef DEERTOS_H
#define DEERTOS_H

#include <stdint.h>


typedef struct {
  void* sp;
} OSThread;

typedef void(*OSThreadHandler)();


int OSThreadStart(OSThread* me, OSThreadHandler threadHandler, void* stkSto, uint32_t stkSize);
int OSInit();
void OSSched();

#endif
