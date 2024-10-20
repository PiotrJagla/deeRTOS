#include "gpio.h"
#include "stm32f302x8.h"

uint8_t pin_to_pudpr[] = {
  GPIO_PUPDR_PUPDR0_Pos,
  GPIO_PUPDR_PUPDR1_Pos,
  GPIO_PUPDR_PUPDR2_Pos,
  GPIO_PUPDR_PUPDR3_Pos,
  GPIO_PUPDR_PUPDR4_Pos,
  GPIO_PUPDR_PUPDR5_Pos,
  GPIO_PUPDR_PUPDR6_Pos,
  GPIO_PUPDR_PUPDR7_Pos,
  GPIO_PUPDR_PUPDR8_Pos,
  GPIO_PUPDR_PUPDR9_Pos,
  GPIO_PUPDR_PUPDR10_Pos,
  GPIO_PUPDR_PUPDR11_Pos,
  GPIO_PUPDR_PUPDR12_Pos,
  GPIO_PUPDR_PUPDR13_Pos,
  GPIO_PUPDR_PUPDR14_Pos,
  GPIO_PUPDR_PUPDR15_Pos,
};
uint8_t pin_to_moder[] = {
  GPIO_MODER_MODER0_Pos,
  GPIO_MODER_MODER1_Pos,
  GPIO_MODER_MODER2_Pos,
  GPIO_MODER_MODER3_Pos,
  GPIO_MODER_MODER4_Pos,
  GPIO_MODER_MODER5_Pos,
  GPIO_MODER_MODER6_Pos,
  GPIO_MODER_MODER7_Pos,
  GPIO_MODER_MODER8_Pos,
  GPIO_MODER_MODER9_Pos,
  GPIO_MODER_MODER10_Pos,
  GPIO_MODER_MODER11_Pos,
  GPIO_MODER_MODER12_Pos,
  GPIO_MODER_MODER13_Pos,
  GPIO_MODER_MODER14_Pos,
  GPIO_MODER_MODER15_Pos,
};

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

void GpioSetPinMode(GPIO_TypeDef* gpiox, uint8_t gpioPin, uint8_t mode) {
  gpiox->MODER |= (mode << pin_to_moder[gpioPin]);
}

void GpioTogglePin(GPIO_TypeDef* gpiox, uint8_t gpioPin) {
  gpiox->ODR ^= (1 << gpioPin);
}

void GpioSetPin(GPIO_TypeDef* gpiox, uint8_t gpioPin) {
  gpiox->ODR |= (1 << gpioPin);
}

void GpioResetPin(GPIO_TypeDef* gpiox, uint8_t gpioPin) {
  gpiox->ODR &= ~(1 << gpioPin);
}

void GpioSetPull(GPIO_TypeDef* gpiox, unsigned long gpioPin, uint8_t mode) {
  gpiox->PUPDR |= (mode<<pin_to_pudpr[gpioPin]);
}

void GpioWritePin(GPIO_TypeDef* gpiox, uint8_t gpioPin, bool value) {
  if(value) {
    GpioSetPin(gpiox, gpioPin);
  } else {
    GpioResetPin(gpiox, gpioPin);
  }
}

bool GpioReadPin(GPIO_TypeDef* gpiox, uint8_t gpioPin) {
  return gpiox->IDR & (1<<gpioPin);
}

void enable_spi() {
  GPIOC->MODER |= ((0b10) << GPIO_MODER_MODER10_Pos);
  GPIOC->MODER |= ((0b10) << GPIO_MODER_MODER11_Pos);
  GPIOC->MODER |= ((0b10) << GPIO_MODER_MODER12_Pos);

  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11;
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12;

  GPIOC->OTYPER &= ~GPIO_OTYPER_OT_10;
  GPIOC->OTYPER &= ~GPIO_OTYPER_OT_11;
  GPIOC->OTYPER &= ~GPIO_OTYPER_OT_12;

  GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR10;
  GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR11;
  GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR12;

  GPIOC->AFR[1] |= (6 << GPIO_AFRH_AFRH2_Pos);
  GPIOC->AFR[1] |= (6 << GPIO_AFRH_AFRH3_Pos);
  GPIOC->AFR[1] |= (6 << GPIO_AFRH_AFRH4_Pos);
}

void enable_i2c() {
  GPIOB->MODER |= ((0b10) << GPIO_MODER_MODER8_Pos);
  GPIOB->MODER |= ((0b10) << GPIO_MODER_MODER9_Pos);

  //Enable pull ups
  GPIOB->PUPDR |= ((0b01) << GPIO_PUPDR_PUPDR8_Pos);
  GPIOB->PUPDR |= ((0b01) << GPIO_PUPDR_PUPDR9_Pos);
  
  GPIOB->AFR[1] |= (4 << GPIO_AFRH_AFRH0_Pos);
  GPIOB->AFR[1] |= (4 << GPIO_AFRH_AFRH1_Pos);
}

void EnableTim16_CH1_PB8() {
  GpioSetPinMode(GPIOB, GPIO_MODER_MODER8_Pos, GPIO_ALTERNATE_FUNCTION);
  GPIOB->AFR[1] &= ~(GPIO_AFRH_AFRH0);
  GPIOB->AFR[1] |= (1<< GPIO_AFRH_AFRH0_Pos);
}

void EnableADC_IN1_PA0() {
  GpioSetPinMode(GPIOA, GPIO_MODER_MODER0_Pos, GPIO_ANALOG_MODE);
}

void EnableADC_IN2_PA1() {
  GpioSetPinMode(GPIOA, GPIO_MODER_MODER1_Pos, GPIO_ANALOG_MODE);
}

void EnableADC_IN6_PC0() {
  GpioSetPinMode(GPIOC, GPIO_MODER_MODER0_Pos, GPIO_ANALOG_MODE);
}

void EnableADC_IN7_PC1() {
  GpioSetPinMode(GPIOC, GPIO_MODER_MODER1_Pos, GPIO_ANALOG_MODE);
}


void GpioLD2EnableOutput(){
  GpioSetPinMode(GPIO_LD2_BASE, GPIO_MODER_MODER13_Pos, GPIO_OUTPUT);
}






