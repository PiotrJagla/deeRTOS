#include "deeRTOS.h"
#include "stm32f3xx.h"

void OS_sched();

#define MAX_THREADS 32
OSThread* volatile OS_curr_task;
OSThread* volatile OS_next_task;
OSThread* OS_threads[MAX_THREADS+1];
uint8_t OS_threads_num = 0;
uint8_t OS_curr_thread_idx = -1;
uint32_t OS_thread_ready_msk = 0;


uint32_t stack_idleThread[20];
OSThread idleThread;
void OS_idle() {
  while(1) {
    __asm__("nop");
  }
}

int OS_init() {
  OS_create_thread(&idleThread, 0, &OS_idle, stack_idleThread, sizeof(stack_idleThread));
  OS_curr_task = (void*)0;
  OS_next_task = (void*)0;

  *(uint32_t volatile *)0xE000ED20 |= (0xFF << 16); //Set pendSV priority to lowest
  return 0;
}

void OS_start() {
  __disable_irq();
  OS_sched();
  __enable_irq();
}


void OS_delay(uint32_t miliseconds){
  __disable_irq();
  OS_curr_task->timeout = miliseconds;
  OS_thread_ready_msk &= ~(1<< (OS_curr_thread_idx - 1));
  OS_sched();
  __enable_irq();
}


void insert_sorted(OSThread* t) {
  OS_threads[OS_threads_num] = t;

  for(int i = OS_threads_num ; i > 1 ; --i) {
    if(OS_threads[i]->priority > OS_threads[i-1]->priority) {
      OSThread* tmp = OS_threads[i];
      OS_threads[i] = OS_threads[i-1];
      OS_threads[i-1] = tmp;
    }
  }
}
int OS_create_thread(OSThread* me, uint8_t priority, OSThreadHandler threadHandler, void* stkSto, uint32_t stkSize) {

  if(OS_threads_num == MAX_THREADS) {
    return 1;
  }

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
  me->priority = priority;

  uint32_t* stk_limit = (uint32_t*)(((((uint32_t)stkSto-1U)/8)+1U)*8);

  for(sp = sp-1U; sp >= stk_limit; --sp) {
    *sp = 0xDEADBEEF;
  }

  //OS_threads[OS_threads_num] = me;
  insert_sorted(me);
  if(OS_threads_num > 0) {
    OS_thread_ready_msk |= (1 << (OS_threads_num - 1));
  }

  OS_threads_num++;
  return 0;
}

void OS_tick() {
  for(int i = 1 ; i < OS_threads_num ; ++i) {
    if(OS_threads[i]->timeout != 0) {
      --OS_threads[i]->timeout;
      if(OS_threads[i]->timeout == 0) {
        OS_thread_ready_msk |= (1 << (i-1));
      }
    }
  }
}

void OS_sched() {
  int next_task_idx = 0;
  if(OS_thread_ready_msk == 0) {
    next_task_idx = 0;
  } else {
    do {
      ++next_task_idx;
      if(next_task_idx == OS_threads_num) {
        next_task_idx = 1;
      }
    } while (!(OS_thread_ready_msk & (1 << (next_task_idx - 1))));
  }
  OS_next_task = OS_threads[next_task_idx];
  OS_curr_thread_idx = next_task_idx;
  *(uint32_t*)0xE000ED04 |= (1 << 28);
}


#ifdef ISRTOS
void systick_handler() {
  OS_tick();
  __disable_irq();
  OS_sched();
  __enable_irq();
}

void pendsv_handler(void) {
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
#endif





