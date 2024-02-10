#include <stm32f302x8.h>
#include <stm32f3xx.h>
#include <stdio.h>
#include <usart.h>
#include <cmsis_util.h>
#include <deeRTOS.h>
#include <gpio.h>

int main(void)
{
  SysTick_Config(CLOCK_FREQ/1000);
  __enable_irq();
  usart_init(USART2);
  GpioInit();
  GpioSetPinMode(GPIOA, GPIO_MODER_MODER9_Pos, GPIO_OUTPUT);
  
  while(1)
  {
    GpioTogglePin(GPIOA, 9);
    delay_ms(1000);
  }
}
