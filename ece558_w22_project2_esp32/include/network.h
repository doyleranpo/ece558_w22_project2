#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <Arduino.h>
#include <WiFi.h>
#include <EspMQTTClient.h>

void wifi_setup();
void mqtt_start();

#endif //__NETWORK_H__