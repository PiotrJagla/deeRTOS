#ifndef PORTMACRO_H
#define PORTMACRO_H

#include <stdint.h>

typedef uint16_t ThreadReadyMask;
#define THREAD_MASK_LAST_BIT 15
#define LAST_PRIORITY THREAD_MASK_LAST_BIT

typedef uint8_t RegisterSize;

#endif
