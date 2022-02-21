#include <Arduino.h>

// User defined includes
#include "common_defs.h"
#include "sensors.h"
#include "button.h"
#include "led.h"
#include "network.h"

void setup() {
  Serial.begin(115200);
  led_setup();
  wifi_setup();
  if(!aht_setup()) {
    Serial.println("Couldn't find sensor");
  }
  set_interval(5);
  button_setup();
}

void loop() {
  update_values();
  read_button();
  mqtt_start();
} 