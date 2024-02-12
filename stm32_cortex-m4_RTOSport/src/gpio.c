#include "gpio.h"
#include "stm32f302x8.h"

void GpioInit(){
  volatile uint32_t dummy;
  RCC->AHBENR |= (1<<RCC_AHBENR_GPIOAEN_Pos);
  RCC->AHBENR |= (1<<RCC_AHBENR_GPIOBEN_Pos);
  RCC->AHBENR |= (1<<RCC_AHBENR_GPIOCEN_Pos);
  RCC->AHBENR |= (1<<RCC_AHBENR_GPIODEN_Pos);
  RCC->AHBENR |= (1<<RCC_AHBENR_GPIOFEN_Pos);
  dummy = RCC->AHBENR;
  dummy = RCC->AHBENR;
}

void GpioSetPinMode(GPIO_TypeDef* gpiox, unsigned long gpioPin, uint8_t mode) {
  gpiox->MODER |= (mode << gpioPin);
}

void GpioTogglePin(GPIO_TypeDef* gpiox, uint8_t gpioPin) {
  gpiox->ODR ^= (1 << gpioPin);
}

void GpioSetPin(GPIO_TypeDef* gpiox, uint8_t gpioPin) {
  gpiox->ODR |= (1 << gpioPin);
}

void GpioResetPin(GPIO_TypeDef* gpiox, uint8_t gpioPin) {
  gpiox->ODR &= (1 << gpioPin);
}

void GpioSetPull(GPIO_TypeDef* gpiox, unsigned long gpioPin, uint8_t mode) {
  gpiox->PUPDR |= (mode<<gpioPin);
}

bool GpioReadPin(GPIO_TypeDef* gpiox, uint8_t gpioPin) {
  return gpiox->IDR & (1<<gpioPin);
}

void EnableTim16_CH1_PB8() {
  GpioSetPinMode(GPIOB, GPIO_MODER_MODER8_Pos, GPIO_ALTERNATE_FUNCTION);
  GPIOB->AFR[1] &= ~(GPIO_AFRH_AFRH0);
  GPIOB->AFR[1] |= (1<< GPIO_AFRH_AFRH0_Pos);
}

void EnableADC_IN1_PA0() {
  GpioSetPinMode(GPIOA, GPIO_MODER_MODER0_Pos, GPIO_ANALOG_MODE);
}




