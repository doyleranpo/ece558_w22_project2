/*****************************
 * common_defs.h
 * author: Ayush Srivastava
 * description: Common definitions
 *              required for the
 *              project
 *****************************/
#ifndef __COMMON_DEFS_H__
#define __COMMON_DEFS_H__

// Timer Parameters
#define TIMER_DEFAULT_INTERVAL      1
#define TIMER_MULTIPLIER            1000000
#define TIMER_CALCULATOR(interval)  (interval * TIMER_MULTIPLIER)

// Button Parameters
#define BUTTON_PIN                  33
#define BUTTON_DEBOUNCE_MS          25          

// LED/PWM Parameters
#define LED_PIN                     12
#define LED_FREQ                    5000
#define LED_CHANNEL                 0
#define LED_RESOLUTION              8

// MQTT Parameters
#define MQTT_BROKER_URI             "broker.hivemq.com"
#define MQTT_PORT                   1883
#define MQTT_CLIENT_NAME            "AyushSriv"
#define MQTT_TOPIC_TEMPERATURE      "ayushsriv/env/temperature"
#define MQTT_TOPIC_HUMIDITY		    "ayushsriv/env/humidity"
#define MQTT_TOPIC_BUTTON		    "ayushsriv/button"
#define MQTT_TOPIC_INTERVAL		    "ayushsriv/env/interval"
#define MQTT_TOPIC_LED		        "ayushsriv/led"
#define MQTT_DEVICE_STATUS          "ayushsriv/devstatus"

#endif //__COMMON_DEFS_H__
