#include "adc.h"
#include <stm32f302x8.h>
#include <stm32f3xx.h>
#include <stdio.h>
#include <usart.h>
#include <cmsis_util.h>
#include <gpio.h>
#include <timers.h>
#include <stdint.h>

void CustomGpioInit();

typedef struct {
  void* sp;
} OSThread;

OSThread* tcb_curr;
OSThread* tcb_next;

typedef void(*OSThreadHandler)();

void OSThreadStart(OSThread* me, OSThreadHandler threadHandler, void* stkSto, uint32_t stkSize) {

  uint32_t* sp = (uint32_t*)((((uint32_t)stkSto + stkSize)/8)*8);

  *(--sp) = (1U << 24); // xPSR thums set
  *(--sp) = (uint32_t)threadHandler; // PC
  *(--sp) = 0x0000000EU; // LR
  *(--sp) = 0x0000000CU; // R12
  *(--sp) = 0x00000003U; // R3
  *(--sp) = 0x00000002U; // R2
  *(--sp) = 0x00000001U; // R1
  *(--sp) = 0x00000000U; // R0
  
  // Save additional registrs
  *(--sp) = 0x0000000BU; // R11
  *(--sp) = 0x0000000AU; // R10
  *(--sp) = 0x00000009U; // R9
  *(--sp) = 0x00000008U; // R8
  *(--sp) = 0x00000007U; // R7
  *(--sp) = 0x00000006U; // R6
  *(--sp) = 0x00000005U; // R5
  *(--sp) = 0x00000004U; // R4
  
  me->sp = sp;
}


uint32_t* sp_next;
uint32_t* sp_curr;

#define STACK_SIZE_TASK1 40
uint32_t stack_task1[STACK_SIZE_TASK1];
uint32_t* sp_task1 = &stack_task1[STACK_SIZE_TASK1];
OSThread tcb_task1;
void task1() {
  while(1) {
    GPIOA->ODR ^= (1<<8);
    for(int i = 0 ; i < 600000; ++i) {}
  }
}

#define STACK_SIZE_TASK2 40
uint32_t stack_task2[STACK_SIZE_TASK2];
uint32_t* sp_task2 = &stack_task2[STACK_SIZE_TASK2];
OSThread tcb_task2;
void task2() {
  while(1) {
    GPIOB->ODR ^= (1<<10);
    for(int i = 0 ; i < 300000; ++i) {}
  }
}


uint32_t ticks = 0;

void systick_handler() {
  ++ticks;

  __asm__("CPSID I");
  
  //Schedule next task


  //Switch context
  if(ticks %2 == 1) {
    tcb_next = &tcb_task1;
  } else {
    tcb_next = &tcb_task2;
  }

  //Save current stack context

  if(ticks != 1) {
    __asm__("POP {r7}");
    __asm__("PUSH {r4-r11}");

    __asm__("LDR r1, =tcb_curr");
    __asm__("LDR r1, [r1, #0x00]");
    __asm__("STR sp, [r1,#0x00]");
  }

  //Restore next task context
  
  // sp = next task stack pointer
  __asm__("LDR r1, =tcb_next");
  __asm__("LDR r1, [r1, #0x00]");
  __asm__("LDR sp, [r1,#0x00]");
  

  // curr stack pointer = next stack pointer
  __asm__("LDR r1, =tcb_next");
  __asm__("LDR r1, [r1, #0x00]");
  __asm__("LDR r2, =tcb_curr");
  __asm__("STR r1, [r2, #0x00]");

  __asm__("POP {r4-r11}");

  __asm__("CPSIE I");
  __asm__("BX LR");

}


int main(void) {
  SysTick_Config(CLOCK_FREQ/1000);
  //SysTick_Config(CLOCK_FREQ/1);
  __enable_irq();
  usart_init(USART2);
  GpioInit();
  CustomGpioInit();


  //*(--sp_task1) |= (1U << 24); // xPSR thumb set
  //*(--sp_task1) = (uint32_t)&task1; // PC
  //*(--sp_task1) = 0x0000000EU; // LR
  //*(--sp_task1) = 0x0000000CU; // R12
  //*(--sp_task1) = 0x00000003U; // R3
  //*(--sp_task1) = 0x00000002U; // R2
  //*(--sp_task1) = 0x00000001U; // R1
  //*(--sp_task1) = 0x00000000U; // R0
  //// Save additional registrs
  //*(--sp_task1) = 0x0000000BU; // R11
  //*(--sp_task1) = 0x0000000AU; // R10
  //*(--sp_task1) = 0x00000009U; // R9
  //*(--sp_task1) = 0x00000008U; // R8
  //*(--sp_task1) = 0x00000007U; // R7
  //*(--sp_task1) = 0x00000006U; // R6
  //*(--sp_task1) = 0x00000005U; // R5
  //*(--sp_task1) = 0x00000004U; // R4
  //
  //*(--sp_task2) |= (1U << 24); // xPSR thumb set
  //*(--sp_task2) = (uint32_t)&task2; // PC
  //*(--sp_task2) = 0x0000000EU; // LR
  //*(--sp_task2) = 0x0000000CU; // R12
  //*(--sp_task2) = 0x00000003U; // R3
  //*(--sp_task2) = 0x00000002U; // R2
  //*(--sp_task2) = 0x00000001U; // R1
  //*(--sp_task2) = 0x00000000U; // R0
  //// Save assitional registers
  //*(--sp_task2) = 0x0000000BU; // R11
  //*(--sp_task2) = 0x0000000AU; // R10
  //*(--sp_task2) = 0x00000009U; // R9
  //*(--sp_task2) = 0x00000008U; // R8
  //*(--sp_task2) = 0x00000007U; // R7
  //*(--sp_task2) = 0x00000006U; // R6
  //*(--sp_task2) = 0x00000005U; // R5
  //*(--sp_task2) = 0x00000004U; // R4

  
  //sp_next = sp_task1;
  //sp_curr = sp_task2;

  OSThreadStart(&tcb_task2, &task2, stack_task2, STACK_SIZE_TASK2);
  OSThreadStart(&tcb_task1, &task1, stack_task1, STACK_SIZE_TASK1);

  tcb_curr = &tcb_task2;
  tcb_next = &tcb_task1;
  //task2();


  while(1) {
    //GPIOB->ODR |= (1<<10);
    //GPIOA->ODR &= ~(1<<8);
  }
}


void CustomGpioInit() {
  GpioLD2EnableOutput();
  GpioSetPinMode(GPIOB, GPIO_MODER_MODER10_Pos, GPIO_OUTPUT);
  GpioSetPinMode(GPIOA, GPIO_MODER_MODER8_Pos, GPIO_OUTPUT);
}
