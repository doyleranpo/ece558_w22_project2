/******************************************
 * button.h
 * author: Ayush Srivastava
 * description: functions required for 
 *              button implementation
 *****************************************/
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>

extern bool button_mqtt_update;

void button_setup();
void read_button();
bool is_pressed();

#endif //__BUTTON_H__