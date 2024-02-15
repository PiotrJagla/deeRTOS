#include "dma.h"
#include "stm32f302x8.h"


void EnableCH1_IT() {
  DMA1_Channel1->CCR |= DMA_CCR_TCIE;
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

void DMA_ADCInit(uint32_t destinationAddr, uint32_t buffSize) {
  //DMA
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;
  //16 bits data
  DMA1_Channel1->CCR |= (0b01 << DMA_CCR_MSIZE_Pos);
  DMA1_Channel1->CCR |= (0b01 << DMA_CCR_PSIZE_Pos);
  //Memory increment
  DMA1_Channel1->CCR |= DMA_CCR_MINC;
  //Enable circular mode
  DMA1_Channel1->CCR |= DMA_CCR_CIRC;

  //number of conversions
  DMA1_Channel1->CNDTR = buffSize;

  DMA1_Channel1->CMAR = destinationAddr;
  DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;


  DMA1_Channel1->CCR |= DMA_CCR_EN;
}
