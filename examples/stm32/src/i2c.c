#include <i2c.h>
#include <gpio.h>


void i2c_init() {
  RCC->APB1ENR |= (1<<RCC_APB1ENR_I2C1EN_Pos);
  enable_i2c();
  
  I2C1->TIMINGR =  0x2000090E & 0xF0FFFFFFU;
  I2C1->OAR1 &= ~I2C_OAR1_OA1EN;
  I2C1->OAR1 = (I2C_OAR1_OA1EN);

  I2C1->CR2 |= (I2C_CR2_AUTOEND);

  I2C1->OAR2 &= ~I2C_OAR2_OA2EN;

  I2C1->CR1 |= I2C_CR1_PE;
}

void i2c_master_transmit(I2C_TypeDef* i2c, uint8_t address, 
                         uint8_t* buff, uint8_t length){
  i2c->CR2 &= ~I2C_CR2_SADD;
  i2c->CR2 |= ((address<<1) << I2C_CR2_SADD_Pos); //Write slave addr
  i2c->CR2 &= ~(1<<I2C_CR2_RD_WRN_Pos); // set read or write
  i2c->CR2 |= (length << I2C_CR2_NBYTES_Pos); //bytes to send

  i2c->CR2 |= (1<<I2C_CR2_START_Pos);
  while(i2c->CR2 & I2C_CR2_START) {}

  while(!(i2c->ISR & (I2C_ISR_TXIS))) {}
  for(int i = 0 ; i < length; ++i) {
    i2c->TXDR = buff[i];
    while(!(i2c->ISR & I2C_ISR_TXE)) {}
  }
}
