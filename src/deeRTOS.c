#include "deeRTOS.h"


#define MAX_THREADS 32
OSThread* volatile tcb_curr;
OSThread* volatile tcb_next;
OSThread* OS_threads[MAX_THREADS+1];
uint8_t OS_threadNum = 0;
uint8_t OS_currThread = -1;

int OSInit() {
  tcb_curr = (void*)0;
  tcb_next = (void*)0;
  return 0;
}

int OSThreadStart(OSThread* me, OSThreadHandler threadHandler, void* stkSto, uint32_t stkSize) {

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

  uint32_t* stk_limit = (uint32_t*)(((((uint32_t)stkSto-1U)/8)+1U)*8);

  for(sp = sp-1U; sp >= stk_limit; --sp) {
    *sp = 0xDEADBEEF;
  }

  if(OS_threadNum < MAX_THREADS) {
    OS_threads[OS_threadNum++] = me;
    return 0;
  } else {
    return 1;
  }

}

void OSSched() {
  tcb_next = OS_threads[++OS_currThread % OS_threadNum];
}


uint32_t ticks = 0;
void systick_handler() {
  ++ticks;

  __asm__("CPSID I");
  
  //Schedule next task
  tcb_next = OS_threads[++OS_currThread % OS_threadNum];

  //Switch context

  //Context switch
  __asm__("LDR r1, =tcb_curr");
  __asm__("LDR r1, [r1, #0x00]");
  __asm__("CBZ r1, Context_Restore");

  //Save current stack context
  __asm__("POP {r7}"); //gcc is pushing this register on stack so i have to pop it
  __asm__("PUSH {r4-r11}");
  __asm__("LDR r1, =tcb_curr");
  __asm__("LDR r1, [r1, #0x00]");
  __asm__("STR sp, [r1,#0x00]");

  //Restore next task context
  __asm__("Context_Restore:");
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
