#include <avr/common.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <stddef.h>
#include <deeRTOS.h>

#include <usartUtils.h>
#include <stdbool.h>

#define BUILTIN_LED PB5
#define GREEN_LED PB4
#define RED_LED PB3
#define YELLOW_LED PB2

#define LIGHT_YELLOW PORTB |= (1<<YELLOW_LED);
#define TOGGLE_YELLOW PORTB ^= (1<<YELLOW_LED);

#define INT1_PIN PD3


#define BLINK1_STACK_SIZE 64
uint8_t blink1_stack[BLINK1_STACK_SIZE] = {};
OSThread blink1_tcb;
void blink1() {
  while(true) {
    PORTB ^= (1<<GREEN_LED);
    OS_delay(2000);
  }
}

#define BLINK2_STACK_SIZE 64
uint8_t blink2_stack[BLINK2_STACK_SIZE] = {};
OSThread blink2_tcb;
void blink2() {
  while(true) {
    PORTB ^= (1<<RED_LED);
    OS_delay(1000);
  }
}

#define BLINK3_STACK_SIZE 64
uint8_t blink3_stack[BLINK3_STACK_SIZE] = {};
OSThread blink3_tcb;
void blink3() {
  while(true) {
    TOGGLE_YELLOW
    OS_delay(500);
  }
}

int main() {
  cli();

  DDRB |= (1<<BUILTIN_LED) | (1<<GREEN_LED) | (1<<RED_LED) | (1<<YELLOW_LED);
  //initUSART();

  OS_init();
  OS_create_thread(&blink1_tcb, 1, &blink1, blink1_stack, 
                sizeof(blink1_stack));
  OS_create_thread(&blink2_tcb, 1, &blink2, blink2_stack, 
                sizeof(blink2_stack));
  OS_create_thread(&blink3_tcb, 1, &blink3, blink3_stack, 
                sizeof(blink3_stack));
  OS_start();


  sei();


  while(1) {
    _delay_ms(100);
    PORTB ^= (1<<BUILTIN_LED);
  }
}


