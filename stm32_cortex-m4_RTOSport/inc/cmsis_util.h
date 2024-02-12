#ifndef CMSIS_UTIL_H
#define CMSIS_UTIL_H

#include <stdint.h>

#define CLOCK_FREQ 8000000U

void delay_ms(uint32_t miliseconds);
void delay_ms_tim6(uint32_t miliseconds);

#endif
