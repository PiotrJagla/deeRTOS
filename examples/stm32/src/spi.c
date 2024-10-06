#include <spi.h>
#include <gpio.h>
#include <stm32f3xx.h>

void spi_init() {
  RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
  enable_spi();

  SPI3->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI; //enable software CS managment
  SPI3->CR1 |= SPI_CR1_MSTR; //Master mode
  SPI3->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
  SPI3->CR1 |= (0b000 << SPI_CR1_BR_Pos);

  SPI3->CR2 |= (0b0111 << SPI_CR2_DS_Pos); //8 bits data size
  SPI3->CR2 |= SPI_CR2_FRXTH;
  
  SPI3->CR1 |= SPI_CR1_SPE;
}

void spi_master_transmit(uint8_t data) {
  while(!(SPI3->SR & SPI_SR_TXE)) {}
  SPI3->DR = data;
}
