#include "adc.h"
#include <stm32f302x8.h>
#include <stm32f3xx.h>
#include <stdio.h>
#include <usart.h>
#include <cmsis_util.h>
#include <gpio.h>
#include <timers.h>
#include <stdint.h>
#include <deeRTOS.h>
#include <i2c.h>

void CustomGpioInit();

#define STACK_SIZE_TASK1 50
uint32_t stack_task1[STACK_SIZE_TASK1];
OSThread tcb_task1;
void task1() {
  while(1) {
    GpioTogglePin(GPIOA, 8);
    delay_ms(1600);
  }
}

#define STACK_SIZE_TASK2 50
uint32_t stack_task2[STACK_SIZE_TASK2];
OSThread tcb_task2;
void task2() {
  while(1) {
    GpioTogglePin(GPIOB, 10);
    delay_ms(800);
  }
}

#define STACK_SIZE_TASK3 50
uint32_t stack_task3[STACK_SIZE_TASK3];
OSThread tcb_task3;
void task3() {
  while(1) {
    GpioTogglePin(GPIO_LD2_BASE, GPIO_LD2_PIN);
    delay_ms(400);
  }
}


int main(void) {
  SysTick_Config(CLOCK_FREQ/1000);
  usart_init(USART2);
  GpioInit();
  CustomGpioInit();
  //TimInit(TIM6, 8000, 1000);
  //TimEnebaleUpdateInterrupts(TIM6, TIM6_DAC_IRQn);
  //TimStart(TIM6);


  //OSThreadStart(&tcb_task1, &task1, stack_task1, sizeof(stack_task1));
  //OSThreadStart(&tcb_task2, &task2, stack_task2, sizeof(stack_task2));
  //OSThreadStart(&tcb_task3, &task3, stack_task3, sizeof(stack_task3));
  //OSInit();
  __enable_irq();


  while(1) {
    delay_ms(100);
    GpioTogglePin(GPIO_LD2_BASE, GPIO_LD2_PIN);
  }
}


void CustomGpioInit() {
  GpioLD2EnableOutput();
  GpioSetPinMode(GPIOB, GPIO_MODER_MODER10_Pos, GPIO_OUTPUT);
  GpioSetPinMode(GPIOA, GPIO_MODER_MODER8_Pos, GPIO_OUTPUT);
}
