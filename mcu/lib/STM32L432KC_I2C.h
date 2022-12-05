#ifndef STM32L4_I2C_H
#define STM32L4_I2C_H

#include <stm32l432xx.h>
#include <stdbool.h>
#include "STM32L432KC_GPIO.h"


#define I2C_SDA PA10
#define I2C_SCL PA9

void init_I2C();

void single_write(char addr, char index, char data);

bool write_check(char addr, char index, char data);

char single_read(char addr, char index);

bool read_check(char addr, char index, char * data);

#endif //STM32L4_I2C_H