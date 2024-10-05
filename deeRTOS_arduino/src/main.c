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

#define LIGHT_YELLOW PORTB |= (1<<YELLOW_LED);
#define TOGGLE_YELLOW PORTB ^= (1<<YELLOW_LED);

#define INT1_PIN PD3


//extern void OS_context_switch(void);
void OS_context_switch(void);


#define BLINK1_STACK_SIZE 64
uint8_t blink1_stack[BLINK1_STACK_SIZE] = {};
void* volatile blink1_sp;
void blink1() {
  while(true) {
    PORTB ^= (1<<GREEN_LED);
    _delay_ms(1000);
  }
}

#define BLINK2_STACK_SIZE 64
uint8_t blink2_stack[BLINK2_STACK_SIZE] = {};
void* volatile blink2_sp;
uint32_t counter = 0;
void blink2() {
  while(true) {
    PORTB ^= (1<<RED_LED);
    _delay_ms(500);
  }
}

void* volatile curr_task_sp;
void* volatile next_task_sp;
int next_task_num = 0;
int curr_task_num = 0;
void* volatile * stack_pointers[32] = {};

void trigger_context_switch() {
  PORTD |= (1<<INT1_PIN);
}

uint32_t ticks = 0;
ISR(TIMER1_OVF_vect)
{
  cli();
  ticks++;
  TCNT1 = 65535 - (F_CPU/256)/1000;

  //schedule next task
  next_task_num = (curr_task_num+1)%2;
  next_task_sp = *stack_pointers[next_task_num];
  sei();

  trigger_context_switch();
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
  
  //Contigure INT1 interrupt
  SREG |= (1<<SREG_I);
  EICRA |= (0b11 << ISC10);
  EIMSK |= (1<<INT1);
  DDRD |= (1<<INT1_PIN);



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
  blink1_sp = sp1;
  stack_pointers[0] = &blink1_sp;

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
  blink2_sp = sp2;
  stack_pointers[1] = &blink2_sp;

  curr_task_sp = NULL;
  next_task_sp = NULL;
  next_task_num = 0;
  curr_task_num = 0;

  sei();


  while(1) {
    _delay_ms(1000);
    PORTB ^= (1<<BUILTIN_LED);
    PORTD |= (1<<INT1_PIN);
  }
}


