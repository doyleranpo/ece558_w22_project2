#include "network.h"
#include "common_defs.h"
#include "led.h"
#include "sensors.h"

const char *ssid = "Y.A.A.S.2.4";
const char *pwwd = "Storyline@809"; 

static EspMQTTClient feather(
    "broker.hivemq.com",
    1883,
    "AyushSriv"
);

void wifi_setup() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pwwd);
    while(WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(150);
        digitalWrite(LED_BUILTIN, LOW);
        delay(150);
    }
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println(WiFi.localIP());
}

void mqtt_start() {
    feather.loop();
}

void led_callback(const String& msg) {
    uint8_t brightness = msg.toInt();
    Serial.println("In LED callback");
    led_set(brightness);
}

void interval_callback(const String& msg) {
    uint16_t interval = msg.toInt();
    Serial.println("In interval callback");
    set_interval(interval);
}

void onConnectionEstablished() {
    Serial.println("In MQTT");
    feather.subscribe("ayushsriv/led", led_callback);
    feather.subscribe("ayushsriv/env/interval", interval_callback);


}


