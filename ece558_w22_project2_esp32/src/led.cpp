/*****************************
 * led.cpp
 * author: Ayush Srivastava
 * description: Function implementationd
 *              for LED using PWM
 *****************************/
#include "led.h"
#include "common_defs.h"

//static bool led_setup_status = false;
static uint8_t led_status = 0;

void led_setup() {
    ledcSetup(LED_CHANNEL, LED_FREQ, LED_RESOLUTION);
    ledcAttachPin(LED_PIN, LED_CHANNEL);
    pinMode(LED_BUILTIN, OUTPUT);
    //led_setup_status = true;
}

uint8_t is_led_active() {
    return led_status;
}

void led_set(uint8_t brightness) {
    ledcWrite(LED_CHANNEL, brightness);
    led_status = brightness;
}
