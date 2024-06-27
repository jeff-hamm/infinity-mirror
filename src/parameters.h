#ifndef _PARAMETERS_H
#define _PARAMETERS_H
#include <Arduino.h>
#include <FastLED.h>

#ifndef LED_TYPE
#define LED_TYPE    WS2811
#endif
#ifndef COLOR_ORDER
#define COLOR_ORDER GRB
#endif
#ifndef LED_DATA_PIN
#define LED_DATA_PIN 2
#endif
#ifndef W_MAX_COUNT
#define W_MAX_COUNT  2          //Number of simultaneous waves
#endif
#ifndef NUM_LEDS
#define NUM_LEDS    16
#endif
#ifndef W_DEFAULT_MODE
#define W_DEFAULT_MODE LED_MODE_AURORA
#endif

typedef enum LedMode {
    LED_MODE_AURORA = 0,
    LED_MODE_WAVES =  1
} LedMode_t;

struct LedParameters {
    uint8_t brightness;
    unsigned long fps;
    uint8_t gradientCount;
    uint8_t minSpeedFactor;
    uint8_t maxSpeedFactor;
    uint8_t maxWaveSpeed;
    uint8_t baseSpeed;
    uint8_t minWidth;
    uint8_t maxWidth;
    uint16_t ttl;
    uint8_t minAlpha;
    uint8_t maxAlpha;
    uint8_t intensity;
    CRGBPalette16 pallette;
    LedMode_t mode;
};
extern LedParameters Parameters;

#endif