#include <usartUtils.h>
#include <avr/io.h>

void initUSART(void) {  
  UBRR0H = (BAUD>>8);  
  UBRR0L = BAUD;       

  UCSR0A |= (1 << U2X0);
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_transmit(unsigned char data) {

  while(!(UCSR0A & (1<<UDRE0))){}

  UDR0 = data;
}

void transmitMessage(char* message) {
  for(int i = 0 ; message[i] != '\0' ; ++i){
    USART_transmit(message[i]);
  }
}


