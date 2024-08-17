#include "deeRTOS.h"
#include "stm32f3xx.h"


#define MAX_THREADS 32
OSThread* volatile tcb_curr;
OSThread* volatile tcb_next;
OSThread* OS_threads[MAX_THREADS+1];
uint8_t OS_threadNum = 0;
uint8_t OS_currThreadIdx = -1;
uint32_t OS_threadsReady = 0;


uint32_t stack_idleThread[20];
OSThread idleThread;
void idle_thread_loop() {
  while(1) {
    __asm__("nop");
  }
}

int OSInit() {
  OSThreadStart(&idleThread, &idle_thread_loop, stack_idleThread, sizeof(stack_idleThread));
  tcb_curr = (void*)0;
  tcb_next = (void*)0;

  *(uint32_t volatile *)0xE000ED20 |= (0xFF << 16); //Set pendSV priority to lowest
  return 0;
}

void OS_start() {
  __disable_irq();
  OSSched();
  __enable_irq();
}

void OS_tick() {
  for(int i = 1 ; i < OS_threadNum ; ++i) {
    if(OS_threads[i]->timeout != 0) {
      --OS_threads[i]->timeout;
      if(OS_threads[i]->timeout == 0) {
        OS_threadsReady |= (1 << (i-1));
      }
    }
  }
}

void OS_delay(uint32_t miliseconds){
  __disable_irq();
  tcb_curr->timeout = miliseconds;
  OS_threadsReady &= ~(1<< (OS_currThreadIdx - 1));
  OSSched();
  __enable_irq();
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

  if(OS_threadNum == MAX_THREADS) {
    return 1;
  }

  OS_threads[OS_threadNum] = me;
  if(OS_threadNum > 0) {
    OS_threadsReady |= (1 << (OS_threadNum - 1));
  }

  OS_threadNum++;
  return 0;
}

void OSSched() {
  if(OS_threadsReady == 0U) {
    OS_currThreadIdx = 0U;
  } else {
    do {
      ++OS_currThreadIdx;
      if(OS_currThreadIdx == OS_threadNum) {
        OS_currThreadIdx = 1;
      }
    } while (!(OS_threadsReady & (1 << (OS_currThreadIdx - 1))));
  }
  tcb_next = OS_threads[OS_currThreadIdx];
  *(uint32_t*)0xE000ED04 |= (1 << 28);
}


#ifdef ISRTOS
void systick_handler() {
  OS_tick();
  __disable_irq();
  OSSched();
  __enable_irq();
}

void pendsv_handler(void) {
  __asm__("CPSID I");

  //arm-none-eabi-gcc is pushing this register on stack so i have to pop it
  //but when i call a function from this interrupt gcc is pushing r7 with lr registers
  __asm__("POP {r7}");

  //Check if current task is null
  __asm__("LDR r1, =tcb_curr");
  __asm__("LDR r1, [r1, #0x00]");
  __asm__("CBZ r1, Context_Restore");

  //Save current stack context
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
#endif




