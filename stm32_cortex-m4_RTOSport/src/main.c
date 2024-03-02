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
    delay_ms(300);
    printf("taks1\n\r");
  }
}

void thread2() {
  while(1) {
    delay_ms(300);
    printf("taks2\n\r");
  }
}

int main(void) {
  SysTick_Config(CLOCK_FREQ/1000);
  __enable_irq();
  usart_init(USART2);
  GpioInit();
  CustomGpioInit();



 
  while(1) {
  }
}


void CustomGpioInit() {
  GpioLD2EnableOutput();
  GpioSetPinMode(GPIOB, GPIO_MODER_MODER10_Pos, GPIO_OUTPUT);
  GpioSetPinMode(GPIOA, GPIO_MODER_MODER8_Pos, GPIO_OUTPUT);
}
