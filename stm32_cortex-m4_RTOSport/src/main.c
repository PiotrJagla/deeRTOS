#include "adc.h"
#include <stm32f302x8.h>
#include <stm32f3xx.h>
#include <stdio.h>
#include <usart.h>
#include <cmsis_util.h>
#include <gpio.h>
#include <timers.h>


void CustomGpioInit();

void thread1() {
  while(1) {
    printf("taks1\n\r");
  }
}

void thread2() {
  while(1) {
    printf("taks2\n\r");
  }
  
}

int main(void)
{
  SysTick_Config(CLOCK_FREQ/1000);
  __enable_irq();
  usart_init(USART2);
  GpioInit();
  CustomGpioInit();

 

  while(1)
  {

    //GpioTogglePin(GPIO_LD2_BASE, GPIO_LD2_PIN);
    //delay_ms(1000);
  }
}


void CustomGpioInit() {
  GpioLD2EnableOutput();
}
