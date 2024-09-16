#include <avr/common.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <stddef.h>

#include <usartUtils.h>
#include <stdbool.h>

#define BUILTIN_LED PB5
#define GREEN_LED PB4
#define RED_LED PB3
#define YELLOW_LED PB2


#define BLINK1_STACK_SIZE 64
uint8_t blink1_stack[BLINK1_STACK_SIZE] = {};
void* volatile blink1_sp;
void blink1() {
  while(true) {
    PORTB ^= (1<<GREEN_LED);
    _delay_ms(100);
  }
}

#define BLINK2_STACK_SIZE 64
uint8_t blink2_stack[BLINK2_STACK_SIZE] = {};
void* volatile blink2_sp;
void blink2() {
  while(true) {
    PORTB ^= (1<<RED_LED);
    _delay_ms(200);
  }
}

void* volatile curr_task_sp;
void* volatile next_task_sp;
int next_task_num = 0;


uint32_t ticks = 0;
ISR(TIMER1_OVF_vect)
{
  ticks++;
  TCNT1 = 65535 - (F_CPU/256)/1000;

  if(next_task_num == 1) {
    next_task_sp = blink1_sp;
    next_task_num = 2;
  } else if(next_task_num == 2) {
    next_task_sp = blink2_sp;
    next_task_num = 1;
  }

  //save context
  if(curr_task_sp != NULL) {
    __asm__ __volatile__ ("lds   XL, curr_task_sp");
    __asm__ __volatile__ ("lds   XH, curr_task_sp + 1");
    __asm__ __volatile__ ("in   XL, __SP_L__");
    __asm__ __volatile__ ("in   XH, __SP_H__");
  }

  curr_task_sp = next_task_sp;

  //restore context
  __asm__ __volatile__ ("lds   XL, next_task_sp");
  __asm__ __volatile__ ("lds   XH, next_task_sp + 1");
  __asm__ __volatile__ ("out   __SP_L__, XL");
  __asm__ __volatile__ ("out   __SP_H__, XH");
}


int main() {

  cli();
  DDRB |= (1<<BUILTIN_LED) | (1<<GREEN_LED) | (1<<RED_LED) | (1<<YELLOW_LED);
  //initUSART();

  TCNT1 = 65535 - (F_CPU/256)/1000;


  //set the prescaler to 1024
  TCCR1B = (0b100<<CS10);
  TCCR1A = 0x00;

  TIMSK1 = (1<<TOIE1);

  //Prepare blink1 stack
  uint8_t* sp1 = &blink1_stack[BLINK1_STACK_SIZE-1];
  uint16_t pc1 = (uint16_t)&blink1;
  *(sp1--) = (uint8_t)(pc1 & 0x00FF);       
  *(sp1--) = (uint8_t)((pc1 >> 8) & 0x00FF); 
  *(sp1--) = 0; 
  *(sp1--) = 0b10000000;  //SREG
  *(sp1--) = 0; 
  *(sp1--) = 0; 
  *(sp1--) = 0; 
  *(sp1--) = 0; 
  *(sp1--) = 0; 
  *(sp1--) = 0; 
  *(sp1--) = 0; 
  *(sp1--) = 0; 
  *(sp1--) = 0; 
  *(sp1--) = 0; 
  *(sp1--) = 0; 
  blink1_sp = sp1;

  uint8_t* sp2 = &blink2_stack[BLINK2_STACK_SIZE-1];
  uint16_t pc2 = (uint16_t)&blink2;
  *(sp2--) = (uint8_t)(pc2 & 0x00FF);       
  *(sp2--) = (uint8_t)((pc2 >> 8) & 0x00FF); 
  *(sp2--) = 0; 
  *(sp2--) = 0b10000000; //SREG
  *(sp2--) = 0; 
  *(sp2--) = 0; 
  *(sp2--) = 0; 
  *(sp2--) = 0; 
  *(sp2--) = 0; 
  *(sp2--) = 0; 
  *(sp2--) = 0; 
  *(sp2--) = 0; 
  *(sp2--) = 0; 
  *(sp2--) = 0; 
  *(sp2--) = 0; 
  blink2_sp = sp2;

  curr_task_sp = NULL;
  next_task_sp = NULL;
  next_task_num = 1;

  sei();


  while(1) {
    _delay_ms(100);
    PORTB ^= (1<<BUILTIN_LED);
  }
}


