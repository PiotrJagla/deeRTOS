#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/sfr_defs.h>
#include <deeRTOS.h>
#include <stddef.h>
#include <stdint.h>

#define INT1_PIN PD3

extern void* volatile * stack_pointers[32];
void* volatile curr_task_sp;
void* next_task_sp = NULL;

void portOS_trigger_context_switch(void) {
  PORTD |= (1<<INT1_PIN);
}

void portOS_disable_interrupts(void) {
  cli();
}
void portOS_enable_interrupts(void) {
  sei();
}

void portOS_internal_threads_config(OSThread* volatile threads[], uint8_t threads_num) {

  for(uint8_t i = 0 ; i < threads_num ; ++i) {
    stack_pointers[i] = &threads[i]->sp;
  }
}

void OS_hardware_specific_config(void) {
  //set timer
  TCNT1 = 65535 - (F_CPU/256)/1000;
  TCCR1B = (0b100<<CS10);
  TCCR1A = 0x00;
  TIMSK1 = (1<<TOIE1);
  
  //Contigure INT1 interrupt
  SREG |= (1<<SREG_I);
  EICRA |= (0b11 << ISC10);
  EIMSK |= (1<<INT1);
  DDRD |= (1<<INT1_PIN);
}

void portOS_init_stack(uint8_t** sp, OSThreadHandler threadHandler,
                       void* stkSto, uint32_t stkSize) {
  *sp = (uint8_t*)((((uint32_t)stkSto + (stkSize-1))/8)*8);

  uint16_t pc = (uint16_t)threadHandler;
  **sp = (uint8_t)(pc & 0x00FF);
  --(*sp);
  **sp = (uint8_t)((pc>>8) & 0x00FF);
  --(*sp);
  **sp = 0;
  --(*sp);
  **sp = 0b10000000;
  --(*sp);
  **sp = 0;
  --(*sp);
  **sp = 0;
  --(*sp);
  **sp = 0;
  --(*sp);
  **sp = 0;
  --(*sp);
  **sp = 0;
  --(*sp);
  **sp = 0;
  --(*sp);
  **sp = 0;
  --(*sp);
  **sp = 0;
  --(*sp);
  **sp = 0;
  --(*sp);
}

ISR(INT1_vect)
{
  extern OSThread* volatile OS_curr_task;
  extern OSThread* volatile OS_next_task;

  extern uint8_t OS_prev_thread_idx;
  extern uint8_t OS_curr_thread_idx;
  extern int next_task_num;
  extern int curr_task_num;

  cli();
  next_task_sp = OS_next_task->sp;
  PORTD &= ~(1<<INT1_PIN);

  if(OS_curr_task != NULL) {
    __asm__ volatile (
        "in     r30, __SP_L__       \n\t"  
        "in     r31, __SP_H__       \n\t"  
        "sts    curr_task_sp, r30   \n\t"  
        "sts    curr_task_sp+1, r31 \n\t"  
        :
        :
        : "r30", "r31"
    );
    *stack_pointers[OS_prev_thread_idx] = curr_task_sp;
  } 

  OS_curr_task = OS_next_task;
  curr_task_sp = OS_next_task->sp;
  //curr_task_num = next_task_num;

  //restore context
  __asm__ __volatile__ ("lds    r26, next_task_sp");
  __asm__ __volatile__ ("lds    r27, next_task_sp + 1");
  __asm__ __volatile__ ("out    __SP_L__, r26");
  __asm__ __volatile__ ("out    __SP_H__, r27");
  sei();
}
