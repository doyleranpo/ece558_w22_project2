/*****************************
 * led.h
 * author: Ayush Srivastava
 * description: functions to 
 *              for using PWM 
 *              for LED
 *****************************/
#ifndef __LED_H__
#define __LED_H__
#include <Arduino.h>

void led_setup();
uint8_t is_led_active();
void led_set(uint8_t brightness);

#endif //__HID_H__
