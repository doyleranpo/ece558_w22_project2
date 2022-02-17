#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <Adafruit_AHTX0.h>
#include <Arduino.h>

#include "common_defs.h"

bool aht_setup();
void timer_setup();
void set_interval(uint16_t sec);

void read_env();
void update_values();

#endif //__SENSORS_H__