
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "STM32L432KC.h"
#include "VL53L0X.h"

void pulse_done(){
  digitalWrite(PA5, PIO_LOW);
  for(int i = 0; i < 10; i++);
  digitalWrite(PA5, PIO_HIGH);
}

void fire_routine(){

for(int j = 0; j < 2; j++){
  newAngle(TIM_FIRE, 0);
  for(int i = 0; i < 6000000; i++);
  newAngle(TIM_FIRE, 90);
  for(int i = 0; i < 6000000; i++);
  }
  pulse_done();
}

void gate_routine(){
  newAngle(TIM_GATE, 120);
  for(int i = 0; i < 30000000; i++);
  newAngle(TIM_GATE, 0);
  pulse_done();
}

int main(void) {

  configureFlash();  
  configureClock();

  init_I2C();
  pinMode(PA5, GPIO_OUTPUT);
  pinMode(PB5, GPIO_INPUT);
  pinMode(PA11, GPIO_INPUT);
  digitalWrite(PA5, PIO_HIGH);

  initServoTIMs();
  newAngle(TIM_FIRE, 90);

  vl53l0x_init();

  uint16_t range = 0;

  while(1) {
    vl53l0x_read_range_single(&range);

    if(range != VL53L0X_OUT_OF_RANGE){
      int ang = 1.3*(atan((double)range/200.0)* 180) / M_PI;
      if(ang >= 0 && ang <= 180){ 
        newAngle(TIM_AIM, ang); 
      }
    }

    if(digitalRead(PB5)) {
      gate_routine();
    }
   if(digitalRead(PA11)) {
      fire_routine();
    }

  } 
}

