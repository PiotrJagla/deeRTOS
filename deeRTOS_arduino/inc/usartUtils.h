#ifndef USARTUTILS_H
#define USARTUTILS_H

#define BAUD_RATE 115200UL
#define BAUD ((F_CPU)/(BAUD_RATE*8UL)-1)

void initUSART(void); 
void USART_transmit(unsigned char data); 
void transmitMessage(char* message); 

#endif
