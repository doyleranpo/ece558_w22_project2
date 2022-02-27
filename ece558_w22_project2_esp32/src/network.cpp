/********************************
 * network.cpp
 * author: Ayush Srivastava
 * description: Function implementations
 *              for WIFI and MQTT
 ********************************/
#include "network.h"
#include "common_defs.h"
#include "led.h"
#include "sensors.h"
#include "button.h"

const char *ssid = "";
const char *pwwd = ""; 

static EspMQTTClient feather(
    MQTT_BROKER_URI, 
    MQTT_PORT,
    MQTT_CLIENT_NAME
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

    if(button_mqtt_update) {
        button_publish(is_pressed());
        button_mqtt_update = false;
    }

    if(sensor_mqtt_update) {
        temperature_publish(get_temperature());
        humidity_publish(get_humidity());
        sensor_mqtt_update = false;
    }


    feather.loop();
}

void onConnectionEstablished() {
    // Serial.println("In MQTT");
    device_status_publish();

    
    feather.subscribe(MQTT_TOPIC_LED, led_callback);
    feather.subscribe(MQTT_TOPIC_INTERVAL, interval_callback);
}

void led_callback(const String& msg) {
    uint8_t brightness = msg.toInt();
    // Serial.println("In LED callback");
    led_set(brightness);
}

void interval_callback(const String& msg) {
    uint16_t interval = msg.toInt();
    // Serial.println("In interval callback");
    set_interval(interval);
}

void button_publish(bool state) {
    const String button_state = String(state);
    feather.publish(MQTT_TOPIC_BUTTON, button_state);
}

void temperature_publish(float temp) {
    const String temperature = String(temp);
    feather.publish(MQTT_TOPIC_TEMPERATURE, temperature);
}

void humidity_publish(float humid) {
    const String humidity = String(humid);
    feather.publish(MQTT_TOPIC_HUMIDITY, humidity);
}

void device_status_publish() {
    String status = String(get_temperature()) + "," + String(get_humidity()) + ","
                    + String(is_led_active()) + "," + String(return_interval()) + ","
                    + String(is_pressed());
    feather.publish(MQTT_DEVICE_STATUS, status);
}
