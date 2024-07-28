#include <cmsis_util.h>
#include <stm32f3xx.h>
#include <stdbool.h>
#include <stdio.h>
#include <gpio.h>



uint32_t ticks = 0;
#ifndef ISRTOS
void systick_handler() {
  ticks++;
}
#endif

uint32_t tim6_ticks = 0;
void tim6_dac_handler() {
  if(TIM6->SR & TIM_SR_UIF) {
    TIM6->SR  &= ~TIM_SR_UIF;
    tim6_ticks++;
  }
}

void delay_ms_tim6(uint32_t miliseconds) {
  uint32_t start = tim6_ticks;
  uint32_t end= tim6_ticks + miliseconds;
  while(end < start) {
    while(tim6_ticks > start) {}
  }

  while(tim6_ticks < end) {}
}

void delay_ms(uint32_t miliseconds){
  uint32_t start = ticks;
  uint32_t end = start + miliseconds;
  if(end < start) {
    while(ticks > start);
  }

  while(ticks < end);
}
