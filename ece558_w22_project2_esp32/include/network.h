#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <Arduino.h>
#include <WiFi.h>
#include <EspMQTTClient.h>

// WiFi Functions
void wifi_setup();

// MQTT Functions
void mqtt_start();
void led_callback(const String& msg);
void interval_callback(const String& msg);
void onConnectionEstablished();
void temperature_publish(float temp);
void humidity_publish(float humid);
void button_publish(bool state);

#endif //__NETWORK_H__