#include "deeRTOS.h"
#include <stdbool.h>
#include <stddef.h>

extern void portOS_trigger_context_switch(void);
extern void portOS_disable_interrupts(void);
extern void OS_hardware_specific_config(void);
extern void portOS_enable_interrupts(void);
extern void portOS_init_stack(uint32_t** sp, OSThreadHandler threadHandler,
                       void* stkSto, uint32_t stkSize);

void OS_sched();

#define MAX_THREADS 32
#define PRIORITIES 31
OSThread* volatile OS_curr_task;
OSThread* volatile OS_next_task;
OSThread* OS_threads[MAX_THREADS+1];
uint8_t OS_prio_threads_num[PRIORITIES] = {0};
uint8_t OS_curr_prio_idx[PRIORITIES] = {0};
uint8_t OS_threads_num = 0;
uint8_t OS_curr_thread_idx = -1;
uint8_t OS_prev_thread_idx = -1;
uint32_t OS_thread_ready_msk = 0;


uint32_t stack_idleThread[50];
OSThread idleThread;
void OS_idle() {
  while(1) {
    __asm__("nop");
  }
}

void set_ready_on_index(uint32_t i) {
  OS_thread_ready_msk |= (1<<(31-i));
}

void reset_ready_on_index(uint32_t i) {
  OS_thread_ready_msk &= ~(1<<(31-i));
}

bool is_ready_on_index(uint32_t i){
  return OS_thread_ready_msk & (1<<(31-i));
}

int OS_init() {
  OS_create_thread(&idleThread, 31, &OS_idle, stack_idleThread, sizeof(stack_idleThread));
  OS_curr_task = (void*)0;
  OS_next_task = (void*)0;

  OS_hardware_specific_config();
  return 0;
}

void OS_start() {
  for(int i = 0 ; i < OS_threads_num ; ++i) {
    set_ready_on_index(i);
  }
  portOS_disable_interrupts();
  OS_sched();
  portOS_enable_interrupts();
}


void OS_delay(uint32_t miliseconds){
  portOS_disable_interrupts();
  OS_curr_task->timeout = miliseconds;
  reset_ready_on_index(OS_curr_thread_idx);
  OS_sched();
  portOS_enable_interrupts();
}

void insert_sorted(OSThread* t) {
  OS_threads[OS_threads_num] = t;

  for(int8_t i = OS_threads_num ; i > 0 ; --i) {
    if(OS_threads[i]->priority < OS_threads[i-1]->priority) {
      OSThread* tmp = OS_threads[i];
      OS_threads[i] = OS_threads[i-1];
      OS_threads[i-1] = tmp;
    }
  }
}

int OS_create_thread(OSThread* me, uint8_t priority, 
        OSThreadHandler threadHandler, void* stkSto, uint32_t stkSize) {

  if(OS_threads_num == MAX_THREADS) {
    return 1;
  }
  
  portOS_init_stack((uint32_t**)&me->sp, threadHandler, stkSto, stkSize);
  me->priority = priority;
  OS_prio_threads_num[priority]++;

  insert_sorted(me);

  OS_threads_num++;
  return 0;
}

void OS_tick() {
  for(int i = 0 ; i < OS_threads_num -1 ; ++i) {
    if(OS_threads[i]->timeout != 0) {
      --OS_threads[i]->timeout;
      if(OS_threads[i]->timeout == 0) {
        set_ready_on_index(i);
      }
    }
  }
}

void OS_sched() {
  int first_free_task = __builtin_clz(OS_thread_ready_msk);
  if(first_free_task == (OS_threads_num-1)){
    OS_next_task = OS_threads[first_free_task];
    OS_curr_thread_idx = first_free_task;
    portOS_trigger_context_switch();
    return;
  }

  int next_task_idx = 0;
  int curr_prio = -1;
  int curr_prio_offset = -1;
  for(int i = first_free_task ; i < OS_threads_num ; ++i) {
    if(OS_threads[i]->priority != curr_prio) {
      curr_prio = OS_threads[i]->priority;
      curr_prio_offset = i;
    }
    int curr_prio_idx = OS_curr_prio_idx[curr_prio];
    int task_idx = curr_prio_offset + curr_prio_idx;
    OS_curr_prio_idx[curr_prio] = (++curr_prio_idx)%OS_prio_threads_num[curr_prio];
    if(is_ready_on_index(task_idx)) {
      next_task_idx = task_idx;
      break;
    }
  }
  OS_next_task = OS_threads[next_task_idx];

  OS_prev_thread_idx = OS_curr_thread_idx; //for AVR
  OS_curr_thread_idx = next_task_idx;
  portOS_trigger_context_switch();
}


#ifdef ISRTOS
//void systick_handler() {
//  OS_tick();
//  portOS_disable_interrupts();
//  OS_sched();
//  portOS_enable_interrupts();
//}
ISR(TIMER1_OVF_vect)
{
  OS_tick();
  TCNT1 = 65535 - (F_CPU/256)/1000;
  portOS_disable_interrupts();

  //schedule next task
  next_task_num = (curr_task_num+1)%2;
  next_task_sp = *stack_pointers[next_task_num];
  sei();

  trigger_context_switch();
}
#endif





