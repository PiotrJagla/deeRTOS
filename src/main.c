#include "mutex.h"
#include <stm32f302x8.h>
#include <stm32f3xx.h>
#include <stdio.h>
#include <usart.h>
#include <cmsis_util.h>
#include <gpio.h>
#include <timers.h>
#include <stdint.h>
#include <deeRTOS.h>
#include <queue.h>
#include <stddef.h>
#include <mutex.h>

#define LED1_BASE GPIOA
#define LED1_PIN 8
#define LED2_BASE GPIOB
#define LED2_PIN 10
#define LED3_BASE GPIOB
#define LED3_PIN 5

void CustomGpioInit();

OS_queue_handle queue;
OS_mutex_handle mutex;
#define QUEUE_INT_SIZE 5
void* buff[QUEUE_INT_SIZE] = {0};

#define STACK_SIZE_TASK1 50
uint32_t stack_task1[STACK_SIZE_TASK1];
OSThread tcb_task1;
void task1() {
  uint32_t* r = NULL;
  uint32_t e = 23;
  while(1) {
    GpioTogglePin(LED1_BASE, LED1_PIN);
    OS_delay(20);
    //r = (uint32_t*)OS_queue_pend(&queue);
    OS_queue_post(&queue, &e);
  }
}

#define STACK_SIZE_TASK2 50
uint32_t stack_task2[STACK_SIZE_TASK2];
OSThread tcb_task2;
void task2() {
  while(1) {
    //GpioTogglePin(LED2_BASE, LED2_PIN);
    OS_delay(2000);
  }
}

#define STACK_SIZE_TASK3 50
uint32_t stack_task3[STACK_SIZE_TASK3];
OSThread tcb_task3;
void task3() {
  uint32_t e = 12;
  uint32_t* r = NULL;
  while(1) {
    GpioTogglePin(LED3_BASE, LED3_PIN);
    OS_delay(4000);
    //OS_queue_post(&queue, &e);
    r = (uint32_t*)OS_queue_pend(&queue);
  }
}


int main(void) {
  system_init();
  usart_init(USART2);
  GpioInit();
  CustomGpioInit();
  OS_init();


  OS_create_thread(&tcb_task1, 1, &task1, stack_task1, sizeof(stack_task1));
  OS_create_thread(&tcb_task2, 1, &task2, stack_task2, sizeof(stack_task2));
  OS_create_thread(&tcb_task3, 1, &task3, stack_task3, sizeof(stack_task3));
  queue = OS_queue_create(buff, QUEUE_INT_SIZE);

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
