#ifndef _MAIN_H
#define _MAIN_H
//#define DEBUG_SERIAL
//#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <Arduino.h>
#include <FastLED.h>
#include "parameters.h"

#if  defined(USE_WIFI) || defined(MODE_SELECTOR_BUTTON)
#define ENABLE_MODE_SELECT
#endif
#if W_DEFAULT_MODE == LED_MODE_AURORA || defined(ENABLE_MODE_SELECT)
#define ENABLE_AURORA
uint16_t mode_aurora();
#endif
#if W_DEFAULT_MODE == LED_MODE_COLORWAVE || defined(ENABLE_MODE_SELECT)
#define ENABLE_COLORWAVE
uint16_t colorwaves();
#endif 


void alive_delay(unsigned long ms);
extern CRGB leds[];

#endif