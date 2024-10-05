#include <deeRTOS.h>
#include <stm32f3xx.h>
#include <stdint.h>

void pendsv_handler(void) {
	extern OSThread* volatile OS_curr_task;
	extern OSThread* volatile OS_curr_task;
  __asm__("CPSID I");

  //arm-none-eabi-gcc is pushing this register on stack so i have to pop it
  //but when i call a function from this interrupt gcc is pushing r7 with lr registers
  __asm__("POP {r7}");

  //Check if current task is null
  __asm__("LDR r1, =OS_curr_task");
  __asm__("LDR r1, [r1]");
  __asm__("CBZ r1, Context_Restore");

  //Save current stack context
  __asm__("PUSH {r4-r11}");
  __asm__("STR sp, [r1]"); // OS_curr_task is already in r1

  //Restore next task context
  __asm__("Context_Restore:");
  // sp = next task stack pointer
  __asm__("LDR r1, =OS_next_task");
  __asm__("LDR r1, [r1]");
  __asm__("LDR sp, [r1]");

  // curr stack pointer = next stack pointer
  __asm__("LDR r1, =OS_next_task");
  __asm__("LDR r1, [r1]");
  __asm__("LDR r2, =OS_curr_task");
  __asm__("STR r1, [r2]");

  __asm__("POP {r4-r11}");

  __asm__("CPSIE I");
  __asm__("BX LR");
}

void portOS_trigger_context_switch(void) {
  *(uint32_t*)0xE000ED04 |= (1 << 28); //trigger pendSV
}

void portOS_disable_interrupts(void) {
  __disable_irq();
}
void portOS_enable_interrupts(void) {
  __enable_irq();
}

void OS_hardware_specific_config(void) {
  *(uint32_t volatile *)0xE000ED20 |= (0xFF << 16); //Set pendSV priority to lowest
}

void portOS_init_stack(uint32_t** sp, OSThreadHandler threadHandler,
                       void* stkSto, uint32_t stkSize) {
  *sp = (uint32_t*)((((uint32_t)stkSto + stkSize)/8)*8);

  --(*sp);
  **sp = (1U << 24); // xPSR thums set
  --(*sp);
  **sp = (uint32_t)threadHandler; // PC
  --(*sp);
  **sp = 0x0000000EU; // LR
  --(*sp);
  **sp = 0x0000000CU; // R12
  --(*sp);
  **sp = 0x00000003U; // R3
  --(*sp);
  **sp = 0x00000002U; // R2
  --(*sp);
  **sp = 0x00000001U; // R1
  --(*sp);
  **sp = 0x00000000U; // R0

  //e dditional registrs
  --(*sp);
  **sp = 0x0000000BU; // R11
  --(*sp);
  **sp = 0x0000000AU; // R10
  --(*sp);
  **sp = 0x00000009U; // R9
  --(*sp);
  **sp = 0x00000008U; // R8
  --(*sp);
  **sp = 0x00000007U; // R7
  --(*sp);
  **sp = 0x00000006U; // R6
  --(*sp);
  **sp = 0x00000005U; // R5
  --(*sp);
  **sp = 0x00000004U; // R4
}
