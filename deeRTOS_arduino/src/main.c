#include <avr/common.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

#include <usartUtils.h>
#include <stdbool.h>

#define BUILTIN_LED PB5
#define GREEN_LED PB4


#define BLINK1_STACK_SIZE 128
uint8_t blink1_stack[BLINK1_STACK_SIZE] = {};
void* blink1_sp;
void blink1() {
  while(true) {
    PORTB ^= (1<<GREEN_LED);
    _delay_ms(100);
  }
}


uint32_t ticks = 0;
ISR(TIMER1_OVF_vect)
{
  ticks++;

  TIMSK1 &= ~(1<<TOIE1);
  //PORTB |= (1<<BUILTIN_LED);
  __asm__ __volatile__ ("lds    r26, blink1_sp                       \n\t"   \
                        "lds    r27, blink1_sp + 1                   \n\t"   \
                        "ld     r28, x+                                 \n\t"   \
                        "out    __SP_L__, r28                           \n\t"   \
                        "ld     r29, x+                                 \n\t"   \
                        "out    __SP_H__, r29                           \n\t"   \
                        "reti"
                        );
}


int main() {

  cli();
  DDRB |= (1<<DDB5) | (1<<DDB4);
  //initUSART();

  TCNT1 = 65535 - (F_CPU/256)/1000;


  //set the prescaler to 1024
  TCCR1B = (0b100<<CS10);
  TCCR1A = 0x00;

  TIMSK1 = (1<<TOIE1);

  //Prepare blink1 stack
  uint8_t* sp = &blink1_stack[BLINK1_STACK_SIZE-1];
  uint16_t pc = (uint16_t)&blink1;
  *(sp--) = 0x11;       
  *(sp--) = 0x22;       
  *(sp--) = 0x33;       
  *(sp--) = (uint8_t)(pc & 0x00FF);       
  *(sp--) = (uint8_t)((pc >> 8) & 0x00FF); 
  //*(sp--) = 0x00; //r29
  //*(sp--) = 0x00; //r28
  //*(sp--) = 0x00; //r31
  //*(sp--) = 0x00; //r30
  //*(sp--) = 0x00; //r27
  //*(sp--) = 0x00; //r26
  //*(sp--) = 0x00; //r25
  //*(sp--) = 0x00; //r24
  //*(sp--) = 0x00; //r23
  //*(sp--) = 0x00; //r22
  //*(sp--) = 0x00; //r21
  //*(sp--) = 0x00; //r20
  //*(sp--) = 0x00; //r19
  //*(sp--) = 0x00; //r18
  //*(sp--) = 0x00; //r17
  //*(sp--) = 0x00; //r16
  //*(sp--) = 0x00; //r0
  //*(sp--) = 0x00; //SREG
  //*(sp) = 0x00; //r1
  blink1_sp = sp;


  sei();


  while(1) {
    _delay_ms(100);
    PORTB ^= (1<<GREEN_LED);
  }
}
