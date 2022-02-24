#include "button.h"
#include "common_defs.h"

static unsigned long hit_time = 0; 
static bool button_status = false;
static bool last_state = false;

bool button_mqtt_update = false;

void button_setup() {
    pinMode(BUTTON_PIN, INPUT_PULLDOWN);
}

bool is_pressed() {
    return button_status;
}

void read_button() {  
    bool read_pin = digitalRead(BUTTON_PIN);

    if (read_pin != last_state) {
        hit_time = millis();
    }
    
    if (millis() - hit_time > BUTTON_DEBOUNCE_MS) {
        if(button_status != read_pin) {
            button_status = read_pin;
            button_mqtt_update = true;
            Serial.println("Button Pressed");
        }
    }

    last_state = read_pin;
}