#include "timers.h"
#include "stm32f302x8.h"


void TimInit(TIM_TypeDef* timx, uint16_t psc, uint16_t arr) {
  if(timx == TIM16) {
    RCC->APB2ENR |= (1<<RCC_APB2ENR_TIM16EN_Pos);
  } else if(timx == TIM1) {
    RCC->APB2ENR |= (1<<RCC_APB2ENR_TIM1EN_Pos);
  } else if(timx == TIM15) {
    RCC->APB2ENR |= (1<<RCC_APB2ENR_TIM15EN_Pos);
  } else if(timx == TIM17) {
    RCC->APB2ENR |= (1<<RCC_APB2ENR_TIM17EN_Pos);
  } else if(timx == TIM2) {
    RCC->APB1ENR |= (1<<RCC_APB1ENR_TIM2EN_Pos);
  } else if(timx == TIM6) {
    RCC->APB1ENR |= (1<<RCC_APB1ENR_TIM6EN_Pos);
  } else if(timx == TIM2) {
    RCC->APB1ENR |= (1<<RCC_APB1ENR_TIM2EN_Pos);
  } 

  timx->PSC = psc;
  timx->ARR = arr;
}


void TimEnebaleUpdateInterrupts(TIM_TypeDef* timx, IRQn_Type interrupt) {
  timx->DIER |= (1<<TIM_DIER_UIE_Pos);
  NVIC_EnableIRQ(interrupt);
}

void TimDisableUpdateInterrupts(TIM_TypeDef* timx, IRQn_Type interrupt) {
  timx->DIER &= ~(1<<TIM_DIER_UIE_Pos);
  NVIC_DisableIRQ(interrupt);
}

void TimStart(TIM_TypeDef* timx) {
  timx->CR1 |= (1<<TIM_CR1_CEN_Pos);
  while(timx->SR & TIM_SR_UIF_Pos) {}
  timx->SR &= ~(1<<TIM_SR_UIF_Pos);
}

void TimStartPWN(TIM_TypeDef* timx) {
  timx->CCMR1 |= (0b0110 << TIM_CCMR1_OC1M_Pos);
  timx->CCMR1 |= (1<<TIM_CCMR1_OC1PE_Pos);
  timx->CR1 |= (1<<TIM_CR1_ARPE_Pos);
  timx->CCER &= ~(TIM_CCER_CC1P);
  timx->CCER |= (1<<TIM_CCER_CC1E_Pos);
  timx->EGR |= (1<<TIM_EGR_UG_Pos);
  timx->CR1 |= TIM_CR1_URS;
  timx->DIER |= (TIM_DIER_CC1IE | TIM_DIER_UIE);
  timx->CCER |= (1<<TIM_CCER_CC1E_Pos);
}

uint16_t TimGetCounter(TIM_TypeDef* timx) {
  return timx->CNT;
}


