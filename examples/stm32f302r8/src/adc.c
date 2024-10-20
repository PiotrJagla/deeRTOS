#include "adc.h"
#include "stm32f302x8.h"


void ADCInit() {
  RCC->AHBENR |= RCC_AHBENR_ADC1EN;

  //ADC synchronus mode
  ADC1_COMMON->CCR |= ADC_CCR_CKMODE_0;

  ADC1->CR |= ADC_CR_ADVREGEN;
  //for(int i = 0 ; i < 100000 ; ++i) {}
  ADC1->CR |= ADC_CR_ADEN;
  while(!(ADC1->ISR & ADC_ISR_ADRDY));
}


void ADC_DMAConfig() {
  //ADC
  ADCInit();
  //enable ADC DMA
  ADC1->CFGR |= ADC_CFGR_DMAEN;
  //Start in circular mode
  ADC1->CFGR |= ADC_CFGR_DMACFG;

  //Continous conversion mode
  ADC1->CFGR |= ADC_CFGR_CONT;

  //Setup 8-bit resolution
  ADC1->CFGR |= ADC_CFGR_RES_1;

  //Overwrite ADC->DR data when new conversion is complete
  ADC1->CFGR |= ADC_CFGR_OVRMOD;
  
  //Setting sample time
  //ADC1->SMPR1 |= (0b011 << ADC_SMPR1_SMP1_Pos);
  //ADC1->SMPR1 |= (0b011 << ADC_SMPR1_SMP6_Pos);
  //ADC1->SMPR1 |= (0b011 << ADC_SMPR1_SMP7_Pos);

  //set 3 channels
  ADC1->SQR1 |= ((3-1)<<ADC_SQR1_L_Pos);

  ADC1->SQR1 |= (6<<ADC_SQR1_SQ1_Pos);
  ADC1->SQR1 |= (7<<ADC_SQR1_SQ2_Pos);
  ADC1->SQR1 |= (1<<ADC_SQR1_SQ3_Pos);
}

void ADCStart() {
  ADC1->CR |= ADC_CR_ADSTART;
}


uint16_t ADCGetValue() {
  return ADC1->DR;
}

void ADCStartConversion() {
  ADC1->CR |= ADC_CR_ADSTART;
  while(!(ADC1->ISR & ADC_ISR_EOC));
}
