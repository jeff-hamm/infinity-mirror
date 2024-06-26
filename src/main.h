#ifndef _MAIN_H
#define _MAIN_H
#ifdef __AVR_ATtiny85__
#else
//#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "ota.h"
#endif
//#define DEBUG_SERIAL
#include <Arduino.h>
#include <FastLED.h>
uint16_t mode_aurora();
void alive_delay(unsigned long ms);
#define DATA_PIN    2
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    16
#define BRIGHTNESS          255
#define FRAMES_PER_SECOND  120
#define GRADIENT_COUNT 5
#define GRAD_NUM(x) (((255/GRADIENT_COUNT)*x))
extern CRGB leds[];

#endif