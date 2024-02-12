#ifndef TIMERS_H
#define TIMERS_H

#include "stm32f302x8.h"
#include <stdint.h>

void TimInit(TIM_TypeDef* timx, uint16_t psc, uint16_t arr);
void TimStart(TIM_TypeDef* timx);
uint16_t TimGetCounter(TIM_TypeDef* timx);
void TimEnebaleUpdateInterrupts(TIM_TypeDef* timx, IRQn_Type interrupt);
void TimDisableUpdateInterrupts(TIM_TypeDef* timx, IRQn_Type interrupt);
void TimStartPWN(TIM_TypeDef* timx);

#endif
