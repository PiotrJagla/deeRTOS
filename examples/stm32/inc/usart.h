#ifndef USART_H
#define USART_H

#include "stm32f302x8.h"
#include "stm32f3xx.h"

void usart_init(USART_TypeDef* usart);
void usart_transmit(USART_TypeDef* usart, char c);

#endif
