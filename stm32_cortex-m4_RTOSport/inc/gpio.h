#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "stm32f302x8.h"
#include "stm32f3xx.h"

#define GPIO_OUTPUT 0b01
#define GPIO_INPUT 0b00
#define GPIO_ALTERNATE_FUNCTION 0b10
#define GPIO_ANALOG_MODE 0b11


void GpioInit();
void GpioSetPinMode(GPIO_TypeDef* gpiox, uint8_t gpioPin, uint8_t mode);
void GpioTogglePin(GPIO_TypeDef* gpiox, uint8_t gpioPin);
void GpioSetPin(GPIO_TypeDef* gpiox, uint8_t gpioPin);
void GpioResetPin(GPIO_TypeDef* gpiox, uint8_t gpioPin);

#endif
