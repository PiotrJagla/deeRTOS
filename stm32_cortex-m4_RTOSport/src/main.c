#include "adc.h"
#include <stm32f302x8.h>
#include <stm32f3xx.h>
#include <stdio.h>
#include <usart.h>
#include <cmsis_util.h>
#include <gpio.h>

//Control pins
#define TFT_WR_BASE GPIOA
#define TFT_WR_PIN 9
#define TFT_WR_MODER GPIO_MODER_MODER9_Pos

#define TFT_RST_BASE GPIOC
#define TFT_RST_PIN 7
#define TFT_RST_MODER GPIO_MODER_MODER7_Pos

#define TFT_CS_BASE GPIOA
#define TFT_CS_PIN 8
#define TFT_CS_MODER GPIO_MODER_MODER8_Pos

#define TFT_RS_BASE GPIOB
#define TFT_RS_PIN 10
#define TFT_RS_MODER GPIO_MODER_MODER10_Pos

#define TFT_RD_BASE GPIOB
#define TFT_RD_PIN 6
#define TFT_RD_MODER GPIO_MODER_MODER6_Pos

//Data pins
#define TFT_DB0_BASE GPIOB
#define TFT_DB0_PIN 2
#define TFT_DB0_MODER GPIO_MODER_MODER2_Pos

#define TFT_DB1_BASE GPIOB
#define TFT_DB1_PIN 1
#define TFT_DB1_MODER GPIO_MODER_MODER1_Pos

#define TFT_DB2_BASE GPIOA
#define TFT_DB2_PIN 7
#define TFT_DB2_MODER GPIO_MODER_MODER7_Pos

#define TFT_DB3_BASE GPIOA
#define TFT_DB3_PIN 6
#define TFT_DB3_MODER GPIO_MODER_MODER6_Pos

#define TFT_DB4_BASE GPIOA
#define TFT_DB4_PIN 5
#define TFT_DB4_MODER GPIO_MODER_MODER5_Pos

#define TFT_DB5_BASE GPIOA
#define TFT_DB5_PIN 4
#define TFT_DB5_MODER GPIO_MODER_MODER4_Pos

#define TFT_DB6_BASE GPIOB
#define TFT_DB6_PIN 11
#define TFT_DB6_MODER GPIO_MODER_MODER11_Pos

#define TFT_DB7_BASE GPIOB
#define TFT_DB7_PIN 12
#define TFT_DB7_MODER GPIO_MODER_MODER12_Pos

#define TFT_DB8_BASE GPIOB
#define TFT_DB8_PIN 4
#define TFT_DB8_MODER GPIO_MODER_MODER4_Pos

#define TFT_DB9_BASE GPIOA
#define TFT_DB9_PIN 11
#define TFT_DB9_MODER GPIO_MODER_MODER11_Pos

#define TFT_DB10_BASE GPIOB
#define TFT_DB10_PIN 5
#define TFT_DB10_MODER GPIO_MODER_MODER5_Pos

#define TFT_DB11_BASE GPIOA
#define TFT_DB11_PIN 12
#define TFT_DB11_MODER GPIO_MODER_MODER12_Pos

#define TFT_DB12_BASE GPIOB
#define TFT_DB12_PIN 3
#define TFT_DB12_MODER GPIO_MODER_MODER3_Pos

#define TFT_DB13_BASE GPIOC
#define TFT_DB13_PIN 5
#define TFT_DB13_MODER GPIO_MODER_MODER5_Pos

#define TFT_DB14_BASE GPIOA
#define TFT_DB14_PIN 10
#define TFT_DB14_MODER GPIO_MODER_MODER10_Pos

#define TFT_DB15_BASE GPIOC
#define TFT_DB15_PIN 6
#define TFT_DB15_MODER GPIO_MODER_MODER6_Pos

#define TFT_CMD_READ_IDENTIFICATION ((volatile uint8_t)0x04)
#define TFT_CMD_WRITE_BRITNESS ((volatile uint8_t)0x51)


void CustomGpioInit();
void Set8BitBus(uint8_t data);
void WriteCmd8(uint8_t cmd);
void WriteData8(uint8_t data);

void WriteCmd16(uint16_t cmd);
void WriteData16(uint16_t cmd);



int main(void)
{
  SysTick_Config(CLOCK_FREQ/1000);
  __enable_irq();
  usart_init(USART2);
  GpioInit();
  CustomGpioInit();


  //Reset is active low
  GpioSetPin(TFT_RST_BASE, TFT_RST_PIN);
  //CS active low
  GpioSetPin(TFT_CS_BASE , TFT_CS_PIN);

  //set WR as active HIGH
  GpioSetPin(TFT_WR_BASE, TFT_WR_PIN);
  //set RD as acrive HIGH
  GpioSetPin(TFT_RD_BASE, TFT_RD_PIN);


  while(1)
  {
    delay_ms(1000);
    WriteCmd8(TFT_CMD_WRITE_BRITNESS);
    WriteData8(0x20);
  }
}

void WriteCmd8(uint8_t cmd) {
  GpioResetPin(TFT_CS_BASE,TFT_CS_PIN);

  GpioResetPin(TFT_RS_BASE, TFT_RS_PIN);
  GpioResetPin(TFT_WR_BASE, TFT_WR_PIN);
  Set8BitBus(cmd);
  GpioSetPin(TFT_WR_BASE, TFT_WR_PIN);
  GpioSetPin(TFT_RS_BASE, TFT_RS_PIN);

  GpioSetPin(TFT_CS_BASE, TFT_CS_PIN);
}

void WriteCmd16(uint16_t cmd) {
  GpioResetPin(TFT_CS_BASE,TFT_CS_PIN);

  GpioResetPin(TFT_RS_BASE, TFT_RS_PIN);
  GpioResetPin(TFT_WR_BASE, TFT_WR_PIN);
  Set16BitBus(cmd);
  GpioSetPin(TFT_WR_BASE, TFT_WR_PIN);
  GpioSetPin(TFT_RS_BASE, TFT_RS_PIN);

  GpioSetPin(TFT_CS_BASE, TFT_CS_PIN);
}

void WriteData8(uint8_t data) {
  GpioResetPin(TFT_CS_BASE,TFT_CS_PIN);

  GpioResetPin(TFT_WR_BASE, TFT_WR_PIN);
  Set8BitBus(data);
  GpioSetPin(TFT_WR_BASE, TFT_WR_PIN);

  GpioSetPin(TFT_CS_BASE, TFT_CS_PIN);

}

void WriteData16(uint16_t data) {
  GpioResetPin(TFT_CS_BASE,TFT_CS_PIN);

  GpioResetPin(TFT_WR_BASE, TFT_WR_PIN);
  Set16BitBus(data);
  GpioSetPin(TFT_WR_BASE, TFT_WR_PIN);

  GpioSetPin(TFT_CS_BASE, TFT_CS_PIN);

}

void Set8BitBus(uint8_t data){
  GpioWritePin(TFT_DB0_BASE,  TFT_DB0_PIN,  0b1&data);
  GpioWritePin(TFT_DB1_BASE,  TFT_DB1_PIN,  0b10&data);
  GpioWritePin(TFT_DB2_BASE,  TFT_DB2_PIN,  0b100&data);
  GpioWritePin(TFT_DB3_BASE,  TFT_DB3_PIN,  0b1000&data);
  GpioWritePin(TFT_DB4_BASE,  TFT_DB4_PIN,  0b10000&data);
  GpioWritePin(TFT_DB5_BASE,  TFT_DB5_PIN,  0b100000&data);
  GpioWritePin(TFT_DB6_BASE,  TFT_DB6_PIN,  0b1000000&data);
  GpioWritePin(TFT_DB7_BASE,  TFT_DB7_PIN,  0b10000000&data);
  GpioWritePin(TFT_DB8_BASE,  TFT_DB8_PIN,  false);
  GpioWritePin(TFT_DB9_BASE,  TFT_DB9_PIN,  false);
  GpioWritePin(TFT_DB10_BASE, TFT_DB10_PIN, false);
  GpioWritePin(TFT_DB11_BASE, TFT_DB11_PIN, false);
  GpioWritePin(TFT_DB12_BASE, TFT_DB12_PIN, false);
  GpioWritePin(TFT_DB13_BASE, TFT_DB13_PIN, false);
  GpioWritePin(TFT_DB14_BASE, TFT_DB14_PIN, false);
  GpioWritePin(TFT_DB15_BASE, TFT_DB15_PIN, false);
}

void Set16BitBus(uint16_t data){
  GpioWritePin(TFT_DB0_BASE,  TFT_DB0_PIN,  0b1&data);
  GpioWritePin(TFT_DB1_BASE,  TFT_DB1_PIN,  0b10&data);
  GpioWritePin(TFT_DB2_BASE,  TFT_DB2_PIN,  0b100&data);
  GpioWritePin(TFT_DB3_BASE,  TFT_DB3_PIN,  0b1000&data);
  GpioWritePin(TFT_DB4_BASE,  TFT_DB4_PIN,  0b10000&data);
  GpioWritePin(TFT_DB5_BASE,  TFT_DB5_PIN,  0b100000&data);
  GpioWritePin(TFT_DB6_BASE,  TFT_DB6_PIN,  0b1000000&data);
  GpioWritePin(TFT_DB7_BASE,  TFT_DB7_PIN,  0b10000000&data);
  GpioWritePin(TFT_DB8_BASE,  TFT_DB8_PIN,  0b100000000&data);
  GpioWritePin(TFT_DB9_BASE,  TFT_DB9_PIN,  0b1000000000&data);
  GpioWritePin(TFT_DB10_BASE, TFT_DB10_PIN, 0b10000000000&data);
  GpioWritePin(TFT_DB11_BASE, TFT_DB11_PIN, 0b100000000000&data);
  GpioWritePin(TFT_DB12_BASE, TFT_DB12_PIN, 0b1000000000000&data);
  GpioWritePin(TFT_DB13_BASE, TFT_DB13_PIN, 0b10000000000000&data);
  GpioWritePin(TFT_DB14_BASE, TFT_DB14_PIN, 0b100000000000000&data);
  GpioWritePin(TFT_DB15_BASE, TFT_DB15_PIN, 0b1000000000000000&data);
}


void CustomGpioInit() {
  GpioSetPinMode(TFT_RS_BASE, TFT_RS_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_WR_BASE, TFT_WR_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_CS_BASE, TFT_CS_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_RST_BASE, TFT_RST_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_RD_BASE, TFT_RD_MODER, GPIO_OUTPUT);

  GpioSetPinMode(TFT_DB0_BASE, TFT_DB0_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB1_BASE, TFT_DB1_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB2_BASE, TFT_DB2_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB3_BASE, TFT_DB3_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB4_BASE, TFT_DB4_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB5_BASE, TFT_DB5_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB6_BASE, TFT_DB6_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB7_BASE, TFT_DB7_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB8_BASE, TFT_DB8_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB9_BASE, TFT_DB9_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB10_BASE, TFT_DB10_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB11_BASE, TFT_DB11_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB12_BASE, TFT_DB12_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB13_BASE, TFT_DB13_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB14_BASE, TFT_DB14_MODER, GPIO_OUTPUT);
  GpioSetPinMode(TFT_DB15_BASE, TFT_DB15_MODER, GPIO_OUTPUT);
}
