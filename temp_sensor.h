#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <Arduino.h>

extern const uint8_t temp_address;

float temp_read();

#endif