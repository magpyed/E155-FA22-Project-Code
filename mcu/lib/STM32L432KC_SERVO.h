#ifndef STM32L4_SERVO_H
#define STM32L4_SERVO_H

#include <stdint.h>
#include <stm32l432xx.h>
#include "STM32L432KC_GPIO.h"


#define TIM_AIM TIM1
#define PIN_AIM PA8

#define TIM_FIRE TIM15
#define PIN_FIRE PA2

#define TIM_GATE TIM16
#define PIN_GATE PA6


///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

// this includes pin setup
//set apb2 presc to 16
void initServoTIMs();

// do shared timer setup here
void initHelper(TIM_TypeDef * TIMx);

void newAngle(TIM_TypeDef * TIMx, char angle);

#endif