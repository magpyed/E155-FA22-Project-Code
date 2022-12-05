
#include <stm32l432xx.h>
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_SERVO.h"



void initServoTIMs(){
  //making pclk 4 MHz
  RCC->CFGR |= _VAL2FLD(RCC_CFGR_PPRE2, 0b111);

  // GPIO CONFIG
  // Send clock to GPIOA/B
  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN);

  pinMode(PIN_AIM, GPIO_ALT);
  pinMode(PIN_FIRE, GPIO_ALT);
  pinMode(PIN_GATE, GPIO_ALT);

  // tim1 ch1
  GPIOA -> AFR[1] |= _VAL2FLD(GPIO_AFRH_AFSEL8, 1);
  // tim15, ch1
  GPIOA -> AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL2, 14);
  // tim16, ch1
  GPIOA -> AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL6, 14);

  // TIMER CONFIG
  // Send clk to timers
  RCC->APB2ENR |= (RCC_APB2ENR_TIM1EN | RCC_APB2ENR_TIM15EN | RCC_APB2ENR_TIM16EN);
  initHelper(TIM1);
  initHelper(TIM15);
  initHelper(TIM16);


}

void initHelper(TIM_TypeDef * TIMx){
  // set cap/comp to output
  TIMx->CCMR1 &= ~(TIM_CCMR1_CC1S);
  
  // set to PWM1 mode 1
  TIMx->CCMR1 |= _VAL2FLD(TIM_CCMR1_OC1M, 0b0110);

  // enable OC1PE
  TIMx->CCMR1 |= TIM_CCMR1_OC1PE;

  // enable ARPE
  TIMx->CR1 |= TIM_CR1_ARPE;

  // enable CC1E
  TIMx->CCER |= TIM_CCER_CC1E;

  // set prescaler (divides by 2)
  TIMx->PSC &= ~(TIM_PSC_PSC);
  TIMx->PSC |= _VAL2FLD(TIM_PSC_PSC, 4);

  // set auto-reload register
  TIMx->ARR &= ~(TIM_ARR_ARR);
  TIMx->ARR |= _VAL2FLD(TIM_ARR_ARR, 40000);

  // set base angle (cap/comp)
  TIMx->CCR1 &= ~(TIM_CCR1_CCR1);
  TIMx->CCR1 |= _VAL2FLD(TIM_CCR1_CCR1, 1800);

  // enable MOE
  TIMx->BDTR |= TIM_BDTR_MOE;
  
  // enable timer
  TIMx->CR1 |= TIM_CR1_CEN;

  // write update bit
  TIMx->EGR |= TIM_EGR_UG;

}

void newAngle(TIM_TypeDef * TIMx, char angle) {
  int cap_comp = 1800 + (3500/180)*angle;
  TIMx->CCR1 &= ~(TIM_CCR1_CCR1);
  TIMx->CCR1 |= _VAL2FLD(TIM_CCR1_CCR1, cap_comp);
}