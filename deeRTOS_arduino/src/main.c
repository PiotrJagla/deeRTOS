#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

#include <usartUtils.h>
#include <stdbool.h>

#define BUILTIN_LED PB5

uint32_t ticks = 0;
bool b = true;


ISR(TIMER1_OVF_vect)
{
  ticks++;
  if(ticks%1000 == 0) {
    //PORTB ^= (1<<BUILTIN_LED);
    if(b) {
      __asm__ __volatile__("cbi %0, %1"::"I"(_SFR_IO_ADDR(PORTB)),"I"(PB5):);
    } else {
      __asm__ __volatile__("sbi %0, %1"::"I"(_SFR_IO_ADDR(PORTB)),"I"(PB5):);
    }
    b = !b;
  }

  TCNT1 = 65535 - (F_CPU/256)/1000;
}


int main() {

  DDRB |= (1<<DDB5);
  //initUSART();

  TCNT1 = 65535 - (F_CPU/256)/1000;


  //set the prescaler to 1024
  TCCR1B = (0b100<<CS10);
  TCCR1A = 0x00;

  TIMSK1 = (1<<TOIE1);

  sei();


  while(1) {
    _delay_ms(1000);
  }
}
