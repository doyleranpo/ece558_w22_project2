#include "sensors.h"

// Sensor object
static Adafruit_AHTX0 aht;
static sensors_event_t humid, temp; 
static float humidity, temperature;

static hw_timer_t * timer = timerBegin(0, 80, true);;

bool do_sense = true;
bool timer_set = false;

bool sensor_mqtt_update = false;

/**
 * @brief Timer interrupt for enabling read
 */
void IRAM_ATTR onTimer(){
    if (!do_sense)
        do_sense = true;
}

/**
 * @brief Setting up the default timer parameters
 * 
 */
void timer_setup()
{
    if (!timer_set) {
        timer_set = true;
        timerAlarmWrite(timer, TIMER_CALCULATOR(TIMER_DEFAULT_INTERVAL), true);
        timerAttachInterrupt(timer, &onTimer, true);
        timerAlarmEnable(timer);
    }
}

void set_interval(uint16_t sec)
{
    if(timer_set) {
        timerAlarmDisable(timer); 
        timerAlarmWrite(timer, (sec * TIMER_MULTIPLIER), true);
        timerAlarmEnable(timer);
    } else {
        timer_setup();
        Serial.printf("Setting up timer with default read interval of %d seconds\r\n", TIMER_DEFAULT_INTERVAL);
    }
}

bool aht_setup()
{
    if(aht.begin())
        return true;
    
    return false;
}

float get_temperature() {
    return temperature;
}

float get_humidity() {
    return humidity;
}

void read_env() {
    aht.getEvent(&humid, &temp);
    temperature = temp.temperature;
    humidity = humid.relative_humidity;
}

void update_values(){
    if(do_sense) {
        do_sense = false;
        read_env();
        sensor_mqtt_update = true;
        Serial.printf("Temperature %.2f Humidity %.2f\r\n", temperature, humidity);
    }
} 