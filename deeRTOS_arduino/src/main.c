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
void* volatile sp;
int next_task_num = 0;
bool isTrue = false;

uint32_t ticks = 0;
ISR(TIMER1_OVF_vect)
{
  cli();
  ticks++;
  TCNT1 = 65535 - (F_CPU/256)/1000;

  if(next_task_num == 1) {
    next_task_sp = blink2_sp;
    next_task_num = 2;
  } else if(next_task_num == 2) {
    next_task_sp = blink1_sp;
    next_task_num = 1;
  }


  //save context
  //if(curr_task_sp != NULL) {
  if(isTrue) {
    //sp = next_task_num == 1 ? (void*)blink1_sp : (void*)blink2_sp;
    //if(next_task_num == 1) {
    //  __asm__ volatile (
    //      "in %A0, __SP_L__ \n\t"  
    //      "in %B0, __SP_H__ \n\t"  
    //      : "=r" (sp)       
    //  );
    //  curr_task_sp = sp;
    //} else if(next_task_num == 2) {
    //  __asm__ volatile (
    //      "in %A0, __SP_L__ \n\t"  
    //      "in %B0, __SP_H__ \n\t"  
    //      : "=r" (sp)       
    //  );
    //  curr_task_sp = sp;
    //}
    __asm__ volatile (
        "lds    r24, curr_task_sp        \n\t"  // Load the low byte of OS_curr_task into r24
        "lds    r25, curr_task_sp+1      \n\t"  // Load the high byte of OS_curr_task into r25
        "in     r26, __SP_L__            \n\t"  // Load the low byte of the stack pointer (SPL) into r26
        "in     r27, __SP_H__            \n\t"  // Load the high byte of the stack pointer (SPH) into r27
        "st     x+, r26                   \n\t"  // Store r26 (SPL) into the address pointed to by OS_curr_task
        "std    x+, r27                 \n\t"  // Store r27 (SPH) into the address (OS_curr_task + 1)
        :
        : "x" (&curr_task_sp)                   // X register points to OS_curr_task
        : "r24", "r25", "r26", "r27"            // Clobbered registers
    );
  } else {
    isTrue = true;
  }

  curr_task_sp = &next_task_sp;

  //restore context
  __asm__ __volatile__ ("lds    r26, next_task_sp");
  __asm__ __volatile__ ("lds    r27, next_task_sp + 1");
  __asm__ __volatile__ ("out    __SP_L__, r26");
  __asm__ __volatile__ ("out    __SP_H__, r27");
  sei();
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


