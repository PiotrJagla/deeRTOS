#include "adc.h"
#include "stm32f302x8.h"


void ADCInit() {
  //Configure for synchronus mode HCLK/1
  ADC1_COMMON->CCR |= (0b01 << ADC_CCR_CKMODE_Pos);

  //Calibrate in single-ended mode
  ADC1->CR |= ADC_CR_ADCALDIF;

  //Start calibration
  ADC1->CR |= ADC_CR_ADCAL;

  //Enable ADC voltage regulator
  ADC1->CR |= (0b01 << ADC_CR_ADVREGEN_Pos);

  //calibration complete
  ADC1->CR &= ~ADC_CR_ADCAL;

  //Start ADC
  ADC1->CR |= ADC_CR_ADEN;

}

uint16_t ADCGetValue() {
  return ADC1_COMMON->CDR;
}

void ADCStartConversion() {
  while(ADC1->ISR & ~(1<<ADC_ISR_ADRDY_Pos));
  ADC1->ISR |= ADC_ISR_ADRDY;
  ADC1->CR |= ADC_CR_ADSTART;

}
