#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#include <usartUtils.h>

#define BUILTIN_LED PB5


uint32_t ticks = 0;

ISR(TIMER1_OVF_vect)
{
  ticks++;
  if(ticks%5000 == 0) {
    PORTB ^= (1<<BUILTIN_LED);
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
