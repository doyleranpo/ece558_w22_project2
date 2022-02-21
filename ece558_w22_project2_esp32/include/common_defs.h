#ifndef __COMMON_DEFS_H__
#define __COMMON_DEFS_H__

#define TIMER_DEFAULT_INTERVAL      1
#define TIMER_MULTIPLIER            1000000
#define TIMER_CALCULATOR(interval)  (interval * TIMER_MULTIPLIER)

#define BUTTON_PIN                  33
#define BUTTON_DEBOUNCE_MS          25          

#define LED_PIN                     12
#define LED_FREQ                    5000
#define LED_CHANNEL                 0
#define LED_RESOLUTION              8

#endif //__COMMON_DEFS_H__
