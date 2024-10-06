#ifndef PORTMACRO_H
#define PORTMACRO_H

#include <stdint.h>

typedef uint32_t ThreadReadyMask;
#define THREAD_MASK_LAST_BIT 31
#define LAST_PRIORITY THREAD_MASK_LAST_BIT

typedef uint32_t RegisterSize;

#endif
