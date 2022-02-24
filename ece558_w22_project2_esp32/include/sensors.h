#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <Adafruit_AHTX0.h>
#include <Arduino.h>

#include "common_defs.h"

extern bool sensor_mqtt_update; 

bool aht_setup();
void timer_setup();
void set_interval(uint16_t sec);

void read_env();
float get_temperature();
float get_humidity();
void update_values();

#endif //__SENSORS_H__