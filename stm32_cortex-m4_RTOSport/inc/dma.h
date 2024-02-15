#ifndef DMA_H
#define DMA_H

#include <stdint.h>


void DMAEnableCH1Interrupt();
void DMA_ADCInit(uint32_t destinationAddr, uint32_t buffSize);

#endif
