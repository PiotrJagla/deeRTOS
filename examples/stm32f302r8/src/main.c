#include <stm32f302x8.h>
#include <stm32f3xx.h>
#include <usart.h>
#include <cmsis_util.h>
#include <gpio.h>
#include <timers.h>
#include <stdint.h>
#include <deeRTOS.h>
#include <printf.h>

#include <queue.h>

#define LED1_BASE GPIOA
#define LED1_PIN 8
#define LED2_BASE GPIOB
#define LED2_PIN 10
#define LED3_BASE GPIOB
#define LED3_PIN 5

OS_queue_handle queue;
#define QUEUE_SIZE 10
uint8_t* buf[QUEUE_SIZE];

void CustomGpioInit();

#define STACK_SIZE_TASK1 50
uint32_t stack_task1[STACK_SIZE_TASK1];
OSThread tcb_task1;
void task1() {
  uint8_t something = 10;
  while(1) {
    //Queue example
    //OS_delay(1000);
    //OS_queue_post(&queue, &something);
    //GpioTogglePin(LED1_BASE, LED1_PIN);
    
    GpioTogglePin(LED1_BASE, LED1_PIN);
    OS_delay(1000);
  }
}

#define STACK_SIZE_TASK2 50
uint32_t stack_task2[STACK_SIZE_TASK2];
OSThread tcb_task2;
void task2() {
  while(1) {
    //Queue example
    //OS_delay(2000);
    //uint8_t* recv = (uint8_t*)OS_queue_pend(&queue);
    //if(*recv == 10) {
    //  GpioTogglePin(LED2_BASE, LED2_PIN);
    //}
    GpioTogglePin(LED2_BASE, LED2_PIN);
    OS_delay(2000);

  }
}

#define STACK_SIZE_TASK3 50
uint32_t stack_task3[STACK_SIZE_TASK3];
OSThread tcb_task3;
void task3() {
  while(1) {
    GpioTogglePin(LED3_BASE, LED3_PIN);
    OS_delay(4000);
  }
}


void main(void) {
  system_init();
  usart_init(USART2);
  GpioInit();
  CustomGpioInit();
  OS_init();


  OS_create_thread(&tcb_task1, 1, &task1, stack_task1, sizeof(stack_task1));
  OS_create_thread(&tcb_task2, 2, &task2, stack_task2, sizeof(stack_task2));
  OS_create_thread(&tcb_task3, 3, &task3, stack_task3, sizeof(stack_task3));

  queue = OS_create_queue((void**)buf, QUEUE_SIZE);

  __enable_irq();

  OS_start();

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
