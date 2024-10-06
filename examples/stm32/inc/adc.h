#ifndef ADC_H
#define ADC_H

#include "stm32f3xx.h"

void ADCInit();
uint16_t ADCGetValue();
void ADCStartConversion();
void ADC_DMAConfig();
void ADCStart();

#endif

