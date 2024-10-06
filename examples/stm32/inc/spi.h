#ifndef SPI_H
#define SPI_H

#include <stdint.h>

void spi_init();
void spi_master_transmit(uint8_t data);

#endif
