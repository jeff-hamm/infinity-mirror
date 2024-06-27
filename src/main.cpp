#include "main.h"
#include "cserial.h"
#ifdef USE_WIFI
#include "ota.h"
#endif

FASTLED_USING_NAMESPACE
CRGB leds[NUM_LEDS];

void setup() {
  delay(200);
  CSerial.begin(9600);
  #ifdef USE_WIFI
  setupOta();
  #endif
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,LED_DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

   // set master brightness control
  FastLED.setBrightness(Parameters.brightness);
  CSerial.println("Setup Complete");
}
  

void loop()
{
//  Call the current pattern function once, updating the 'leds' array
  #ifdef ENABLE_MODE_SELECT
  if(Parameters.mode == LED_MODE_AURORA)
  #endif
  #ifdef ENABLE_AURORA
    mode_aurora();
  #endif
  #ifdef ENABLE_MODE_SELECT
  else
  #endif
  #ifdef ENABLE_COLORWAVE
    colorwaves();
  #endif
  FastLED.show();  
  alive_delay(1000/Parameters.fps); 

  // do some periodic updatess
//  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
//  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

void alive_delay(unsigned long ms) {
	unsigned long start = millis();
  do {
  #ifndef FASTLED_ACCURATE_CLOCK
		// make sure to allow at least one ms to pass to ensure the clock moves
		// forward
		::delay(1);
#endif
#ifdef USE_WIFI
  loopOta();
#endif
		FastLED.show();
		yield();
	}
	while((millis()-start) < ms);
}
#ifdef ENABLE_COLORWAVE
int step = 1;
int hue0 = 120;
uint16_t colorwaves()
{
  uint16_t duration = Parameters.baseSpeed;
  uint16_t sPseudotime = step;
  uint16_t sHue16 = hue0;

  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  // uint16_t hueinc16 = beatsin88(113, 300, 1500);
  uint16_t hueinc16 = beatsin88(113, 60, 300)*(Parameters.intensity/255);  // Use the Intensity Slider for the hues

  sPseudotime += duration * msmultiplier;
  sHue16 += duration * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;
  CRGB fastled_col;

  for ( uint16_t i = 0; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 >> 8;
    uint16_t h16_128 = hue16 >> 7;
    if ( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = ColorFromPalette(Parameters.pallette, hue8, bri8);
    nblend(leds[(NUM_LEDS-1)-i], newcolor, 128);
  }
  step = sPseudotime;
  hue0 = sHue16;
  return 0;
}
#endif
#ifdef ENABLE_AURORA
uint8_t paletteBlend = 0;
long _dataLen = 0;
CRGB color_from_palette(uint8_t i, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri=255)
{
  uint8_t paletteIndex = i;
  if (mapping && NUM_LEDS > 1) paletteIndex = (i*255)/(NUM_LEDS -1);
  if (!wrap) paletteIndex = scale8(paletteIndex, 240); //cut off blend at palette "end"
  CRGB fastled_col;
  fastled_col = ColorFromPalette(Parameters.pallette, paletteIndex, pbri, (paletteBlend == 3)? NOBLEND:LINEARBLEND);

  return fastled_col;
}


class AuroraWave {
  private:
    uint16_t ttl;
    CRGB basecolor;
    float basealpha;
    uint16_t age;
    uint16_t width;
    float center;
    bool goingleft;
    float speed_factor;
    bool alive = true;

  public:
    void init(uint32_t segment_length, CRGB color) {
      #ifdef DEBUG_SERIAL
      CSerial.println("Init");
      #endif
      ttl = Parameters.ttl;
      basecolor = color;
      basealpha = random(Parameters.minAlpha, Parameters.maxAlpha) / (float)100;
      age = 0;
      width = random(Parameters.minWidth, Parameters.maxWidth); //half of width to make math easier
      if (!width) width = 1;
      if(width > segment_length)
        width = segment_length;
      center = 0;
      goingleft = 0;
      speed_factor = (random(Parameters.minSpeedFactor, Parameters.maxSpeedFactor) / (float)100 * Parameters.maxWaveSpeed / 255);
      alive = true;
    }

    CRGB getColorForLED(int ledIndex) {      
      if(ledIndex < center - width || ledIndex > center + width) return 0; //Position out of range of this wave

      CRGB rgb;

      //Offset of this led from center of wave
      //The further away from the center, the dimmer the LED
      float offset = ledIndex - center;
      if (offset < 0) offset = -offset;
      float offsetFactor = offset / width;

      //The age of the wave determines it brightness.
      //At half its maximum age it will be the brightest.
      float ageFactor = 0.1;        
      if((float)age / ttl < 0.5) {
        ageFactor = (float)age / (ttl / 2);
      } else {
        ageFactor = (float)(ttl - age) / ((float)ttl * 0.5);
      }

      //Calculate color based on above factors and basealpha value
      float factor = (1 - offsetFactor) * ageFactor * basealpha;
      rgb.r = basecolor.r * factor;
      rgb.g = basecolor.g * factor;
      rgb.b = basecolor.b * factor;
    
      return rgb;
    };

    //Change position and age of wave
    //Determine if its sill "alive"
    void update(uint32_t segment_length, uint32_t speed) {
      if(goingleft) {
        center -= speed_factor * speed;
      } else {
        center += speed_factor * speed;
      }

      age++;

      if(age > ttl) {
        alive = false;
      } else {
        if(goingleft) {
          if(center + width < 0) {
            alive = false;
          }
        } else {
          if(center - width > segment_length) {
            alive = false;
          }
        }
      }
    };

    bool stillAlive() {
      if(!alive) {
          #ifdef DEBUG_SERIAL
          CSerial.println("Dead");
          #endif

      }
      return alive;
    };
};
uint8_t aux0 = 0;
uint8_t aux1 = 0;
uint8_t call = 0;
AuroraWave waves[W_MAX_COUNT];

uint16_t mode_aurora(void) {

  if(aux0 != Parameters.intensity || call == 0) {
    //Intensity slider changed or first call
    call++;
    aux1 = W_MAX_COUNT;
    aux0 = Parameters.intensity;
    for(int i = 0; i < aux1; i++) {
      waves[i].init(NUM_LEDS, color_from_palette(random8(), false, false, random(0, 3)));
    }
  }


  for(int i = 0; i < aux1; i++) {
    //Update values of wave
    waves[i].update(NUM_LEDS, Parameters.baseSpeed);

    if(!(waves[i].stillAlive())) {
      //If a wave dies, reinitialize it starts over.
      waves[i].init(NUM_LEDS, color_from_palette(random8(), false, false, random(0, 3)));
    }
  }

  
  uint8_t backlight = 3; //dimmer backlight if less active colors
  for(uint8_t i = 0; i < NUM_LEDS; i++) { 
    CRGB mixedRgb = CRGB(backlight, backlight, backlight);

    //For each LED we must check each wave if it is "active" at this position.
    //If there are multiple waves active on a LED we multiply their values.
    for(int  j = 0; j < aux1; j++) {
      CRGB rgb = waves[j].getColorForLED(i);
      
      if(rgb != CRGB(0)) {       
        mixedRgb += rgb;
      }
    }
    leds[i] = CRGB(mixedRgb[0], mixedRgb[1], mixedRgb[2]);
  }      return 0;

  return 0;
}
#endif