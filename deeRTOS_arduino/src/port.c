#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/sfr_defs.h>
#include <deeRTOS.h>
#include <stddef.h>

#define INT1_PIN PD3

void portOS_trigger_context_switch(void) {
  PORTD |= (1<<INT1_PIN);
}

void portOS_disable_interrupts(void) {
  cli();
}
void portOS_enable_interrupts(void) {
  sei();
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

void portOS_init_stack(uint32_t** sp, OSThreadHandler threadHandler,
                       void* stkSto, uint32_t stkSize) {
}

ISR(INT1_vect)
{
  extern void* volatile curr_task_sp;
  extern void* volatile next_task_sp;

  extern void* volatile * stack_pointers[32];
  extern int curr_task_num;
  extern int next_task_num;

  cli();
  PORTD &= ~(1<<INT1_PIN);
  if(curr_task_sp != NULL) {
    
    __asm__ volatile (
        "in     r30, __SP_L__       \n\t"  
        "in     r31, __SP_H__       \n\t"  
        "sts    curr_task_sp, r30   \n\t"  
        "sts    curr_task_sp+1, r31 \n\t"  
        :
        :
        : "r30", "r31"
    );
    *stack_pointers[curr_task_num] = curr_task_sp;
  } 

  curr_task_sp = next_task_sp;
  curr_task_num = next_task_num;

  //restore context
  __asm__ __volatile__ ("lds    r26, next_task_sp");
  __asm__ __volatile__ ("lds    r27, next_task_sp + 1");
  __asm__ __volatile__ ("out    __SP_L__, r26");
  __asm__ __volatile__ ("out    __SP_H__, r27");
  sei();
}
