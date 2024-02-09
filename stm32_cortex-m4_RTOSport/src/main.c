#include <stm32f302x8.h>
#include <stdint.h>
#include <stm32f3xx.h>
#include <stdio.h>
#include <usart.h>
#include <cmsis_util.h>

int main(void)
{
  SysTick_Config(CLOCK_FREQ/1000);
  __enable_irq();
  usart_init(USART2);
  
  while(1)
  {
    delay_ms(1000);
  }
}
