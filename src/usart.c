#include "usart.h"

void usart_init(USART_TypeDef* usart){
  volatile uint32_t dummy;

  RCC->APB1ENR |= (1<<RCC_APB1ENR_USART2EN_Pos);
  dummy = RCC->APB1ENR;
  dummy = RCC->APB1ENR;

  RCC->AHBENR |= (1 << RCC_AHBENR_GPIOAEN_Pos);
  dummy = RCC->AHBENR;
  dummy = RCC->AHBENR;

  GPIOA->MODER &= ~(GPIO_MODER_MODER2_Msk);
  GPIOA->MODER &= ~(GPIO_MODER_MODER3_Msk);

  GPIOA->MODER |= (0b10 << GPIO_MODER_MODER2_Pos);
  GPIOA->MODER |= (0b10 << GPIO_MODER_MODER3_Pos);

  GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2 | GPIO_AFRL_AFRL3);
  GPIOA->AFR[0] |= (7<< GPIO_AFRL_AFRL2_Pos | 7<< GPIO_AFRL_AFRL3_Pos );

  USART2->BRR = 69; //Baud rate is 9600
  USART2->CR1 |= USART_CR1_UE | USART_CR1_TE;

  USART2->TDR = 0;
  while(!(USART2->ISR & USART_ISR_TC));
}

void usart_transmit(USART_TypeDef *usart, char c) {
  usart->TDR = c;
  while(!(usart->ISR & USART_ISR_TC));
}
