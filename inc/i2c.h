#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include "stm32f3xx.h"

void i2c_init();
void i2c_master_transmit(I2C_TypeDef* i2c, uint8_t address, 
                         uint8_t* buff, uint8_t length);

#endif
