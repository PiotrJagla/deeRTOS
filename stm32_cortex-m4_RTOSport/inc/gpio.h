#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "stm32f302x8.h"
#include "stm32f3xx.h"
#include <stdbool.h>

#define GPIO_OUTPUT 0b01
#define GPIO_INPUT 0b00
#define GPIO_ALTERNATE_FUNCTION 0b10
#define GPIO_ANALOG_MODE 0b11

#define GPIO_PULLUP 0b01
#define GPIO_PULLDOWN 0b10
#define GPIO_NOPULL 0b00
#define GPIO_RESERVED 0b11

#define GPIO_LD2_PIN 13
#define GPIO_LD2_BASE GPIOB


void GpioInit();
void GpioSetPinMode(GPIO_TypeDef* gpiox, unsigned long gpioPin, uint8_t mode);
void GpioTogglePin(GPIO_TypeDef* gpiox, uint8_t gpioPin);
void GpioSetPin(GPIO_TypeDef* gpiox, uint8_t gpioPin);
void GpioResetPin(GPIO_TypeDef* gpiox, uint8_t gpioPin);
void GpioSetPull(GPIO_TypeDef* gpiox, unsigned long gpioPin, uint8_t mode);
bool GpioReadPin(GPIO_TypeDef* gpiox, uint8_t gpioPin);
void GpioWritePin(GPIO_TypeDef* gpiox, uint8_t gpioPin, bool value);

void EnableTim16_CH1_PB8();
void EnableADC_IN1_PA0();
void EnableADC_IN2_PA1();

void GpioLD2EnableOutput();

#endif
