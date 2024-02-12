#include <stm32f302x8.h>
#include <stm32f3xx.h>
#include <stdio.h>
#include <usart.h>
#include <cmsis_util.h>
#include <deeRTOS.h>
#include <gpio.h>
#include <timers.h>

void CustomGpioInit();

int main(void)
{
  SysTick_Config(CLOCK_FREQ/1000);
  __enable_irq();
  usart_init(USART2);
  GpioInit();
  CustomGpioInit();

  EnableADC_IN1_PA0();


  //Enable ADC1 clock
  RCC->AHBENR |= RCC_AHBENR_ADC1EN;

  ADC1_COMMON->CCR |= ADC_CCR_CKMODE_0;

  ADC1->CR |= ADC_CR_ADVREGEN;
  for(int i = 0 ; i < 1000 ; ++i) {}
  ADC1->CR |= ADC_CR_ADEN;
  while(!(ADC1->ISR & ADC_ISR_ADRDY));
  ADC1->SQR1 |= (1<<ADC_SQR1_SQ1_Pos);


  //overwrite data with next conversion
  //ADC1->CFGR |= ADC_CFGR_OVRMOD;


  //Start ADC
  delay_ms(500);

  while(1)
  {
    //ADC conversion 
    ADC1->CR |= ADC_CR_ADSTART;
    while(!(ADC1->ISR & ADC_ISR_EOC));
    uint16_t value = ADC1->DR;
    printf("ADC conversion: %d\n\r", value);

    GpioTogglePin(GPIOB, 8);
    delay_ms(1000);
  }
}

void CustomGpioInit() {
  GpioSetPinMode(GPIOA, GPIO_MODER_MODER8_Pos, GPIO_INPUT);
  GpioSetPull(GPIOA, GPIO_PUPDR_PUPDR8_Pos, GPIO_PULLUP);
  GpioSetPin(GPIOA, 8);

  GpioSetPinMode(GPIOB, GPIO_MODER_MODER8_Pos, GPIO_OUTPUT);

}
