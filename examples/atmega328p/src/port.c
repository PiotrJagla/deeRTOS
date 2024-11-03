#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/sfr_defs.h>

#include <portmacro.h>
#include <deeRTOS.h>


#define INT1_PIN PD3

void* volatile * stack_pointers[32];
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

void portOS_init_stack(RegisterSize** sp, OSThreadHandler threadHandler,
                       void* stkSto, uint32_t stkSize) {
  *sp = (RegisterSize*)((((uint32_t)stkSto + (stkSize-1))/8)*8);

  uint16_t pc = (uint16_t)threadHandler;
  **sp = (uint8_t)(pc & 0x00FF); //PC_L
  --(*sp);
  **sp = (uint8_t)((pc>>8) & 0x00FF); //PC_H
  --(*sp);
  **sp = 0; //R1
  --(*sp);
  **sp = 0b10000000; //SREG
  --(*sp);
  **sp = 0; //R0
  --(*sp);
  **sp = 0; //R18
  --(*sp);
  **sp = 0; //R19
  --(*sp);
  **sp = 0; //R24
  --(*sp);
  **sp = 0; //R25
  --(*sp);
  **sp = 0; //R30
  --(*sp);
  **sp = 0; //R31
  --(*sp);
  **sp = 0; //R28
  --(*sp);
  **sp = 0; //R29
  --(*sp);
}

ISR(INT1_vect)
{
  extern OSThread* volatile OS_curr_task;
  extern OSThread* volatile OS_next_task;
  extern uint8_t OS_prev_thread_idx;
  extern uint8_t OS_curr_thread_idx;

  PORTD &= ~(1<<INT1_PIN);

  cli();
  if(OS_prev_thread_idx != OS_curr_thread_idx) {
    next_task_sp = OS_next_task->sp;

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

    //restore context
    __asm__ __volatile__ ("lds    r26, next_task_sp");
    __asm__ __volatile__ ("lds    r27, next_task_sp + 1");
    __asm__ __volatile__ ("out    __SP_L__, r26");
    __asm__ __volatile__ ("out    __SP_H__, r27");
  }
  sei();
}


ISR(TIMER1_OVF_vect)
{
  extern void OS_tick(void);
  extern void OS_sched(void);

  TCNT1 = 65535 - (F_CPU/256)/1000;
  OS_tick();
  portOS_disable_interrupts();
  OS_sched();
  portOS_enable_interrupts();
}

uint16_t sync_fetch_and_add(volatile uint16_t *ptr, uint16_t value) {
    uint16_t old_value;

    uint8_t sreg = SREG;
    cli();             

    old_value = *ptr;
    *ptr = old_value + value;

    SREG = sreg;

    return old_value;
}

uint16_t sync_fetch_and_sub(volatile uint16_t *ptr, uint16_t value) {
    uint16_t old_value;

    uint8_t sreg = SREG;
    cli();             

    old_value = *ptr;
    *ptr = old_value - value;

    SREG = sreg;

    return old_value;
}

uint16_t sync_val_compare_and_swap(volatile uint16_t *ptr, uint16_t oldval, uint16_t newval) {
  uint16_t original_value;

  uint8_t sreg = SREG;
  cli();

  original_value = *ptr;
  if (original_value == oldval) {
    *ptr = newval;
  }

  SREG = sreg;

  return original_value;
}

bool sync_bool_compare_and_swap(bool * ptr, bool old_val, bool new_val) {
  bool res;
  uint8_t sreg = SREG;
  cli();

  if(*ptr == old_val) {
    *ptr = new_val;
    res = true;
  } else {
    res = false;
  }

  SREG = sreg;

  return res;
}
