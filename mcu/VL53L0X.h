#ifndef VL53L0X_H
#define VL53L0X_H

#include <stdio.h>
#include <stdlib.h>
#include "STM32L432KC.h"
#include <stdbool.h>

// Unless stated otherwise, this implementation is pulled from 
// ArtfulBytes' open source VL53L0X driver.
// here is the specific webpage: www.artfulbytes.com/vl53l0x-post

#define VL53L0X_OUT_OF_RANGE (8190)

// Our function
bool TOF_write(char index, char data);

// Our function
bool TOF_read(char index, char * data);

bool vl53l0x_init();

bool vl53l0x_read_range_single(uint16_t * range);

#endif //VL53L0X_H

