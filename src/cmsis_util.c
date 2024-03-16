#include <cmsis_util.h>
#include <stm32f3xx.h>
#include <stdbool.h>
#include <stdio.h>
#include <gpio.h>


extern uint32_t ticks;



//void systick_handler() {
//  ticks++;
//}
uint32_t tim6_ticks = 0;
void tim6_dac_handler() {
  tim6_ticks++;
  printf("hwllo %d\r\n", tim6_ticks);
}


void delay_ms_tim6(uint32_t miliseconds) {
  TIM6->ARR = miliseconds + 1;
  TIM6->CNT = 0;
  while(TIM6->CNT < miliseconds) {
    __asm__ volatile("NOP");
  }
}

void delay_ms(uint32_t miliseconds){
  uint32_t start = ticks;
  uint32_t end = start + miliseconds;
  if(end < start) {
    while(ticks > start);
  }

  while(ticks < end);
}
