#include <cmsis_util.h>


static uint32_t ticks;

void systick_handler(){
  ticks++;
}

void delay_ms(uint32_t miliseconds){
  uint32_t start = ticks;
  uint32_t end = start + miliseconds;
  if(end < start) {
    while(ticks > start);
  }

  while(ticks < end);
}
