#include <stm32f302x8.h>
#include <stm32f3xx.h>
#include <stdio.h>
#include <usart.h>
#include <cmsis_util.h>
#include <gpio.h>
#include <timers.h>
#include <stdint.h>
#include <deeRTOS.h>

#define LED1_BASE GPIOA
#define LED1_PIN 8
#define LED2_BASE GPIOB
#define LED2_PIN 10
#define LED3_BASE GPIOB
#define LED3_PIN 5

void CustomGpioInit();

#define STACK_SIZE_TASK1 50
uint32_t stack_task1[STACK_SIZE_TASK1];
OSThread tcb_task1;
void task1() {
  while(1) {
    GpioTogglePin(LED1_BASE, LED1_PIN);
    delay_ms_tim6(1000);
  }
}

#define STACK_SIZE_TASK2 50
uint32_t stack_task2[STACK_SIZE_TASK2];
OSThread tcb_task2;
void task2() {
  while(1) {
    GpioTogglePin(LED2_BASE, LED2_PIN);
    delay_ms_tim6(500);
  }
}

#define STACK_SIZE_TASK3 50
uint32_t stack_task3[STACK_SIZE_TASK3];
OSThread tcb_task3;
void task3() {


  while(1) {
    GpioTogglePin(LED3_BASE, LED3_PIN);
    delay_ms_tim6(500);
  }
}

int main(void) {
  SysTick_Config(CLOCK_FREQ/1000);
  usart_init(USART2);
  GpioInit();
  CustomGpioInit();
  TimInit(TIM6, CLOCK_FREQ/100000-1, CLOCK_FREQ/80000-1);
  TimEnebaleUpdateInterrupts(TIM6, TIM6_DAC_IRQn);
  TimStart(TIM6);


  OSThreadStart(&tcb_task1, &task1, stack_task1, sizeof(stack_task1));
  OSThreadStart(&tcb_task2, &task2, stack_task2, sizeof(stack_task2));
  OSThreadStart(&tcb_task3, &task3, stack_task3, sizeof(stack_task3));
  OSInit();


  __enable_irq();

  while(1) {
    delay_ms(1000);
    GpioTogglePin(LED1_BASE, LED1_PIN);
    GpioTogglePin(LED2_BASE, LED2_PIN);
    GpioTogglePin(LED3_BASE, LED3_PIN);
  }
}


void CustomGpioInit() {
  GpioLD2EnableOutput();
  GpioSetPinMode(LED1_BASE, LED1_PIN, GPIO_OUTPUT);
  GpioSetPinMode(LED2_BASE, LED2_PIN, GPIO_OUTPUT);
  GpioSetPinMode(LED3_BASE, LED3_PIN, GPIO_OUTPUT);
}
