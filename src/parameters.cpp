
#include "parameters.h"


#ifndef BRIGHNESS
#define BRIGHTNESS  255
#endif

#ifndef TTL
#define TTL  5000
#endif
#ifndef FRAMES_PER_SECOND
#define FRAMES_PER_SECOND  120
#endif
#ifndef GRADIENT_COUNT
#define GRADIENT_COUNT 5
#endif
#ifndef W_MIN_SPEED_FACTOR
#define W_MIN_SPEED_FACTOR 10          //Higher number, higher speed
#endif
#ifndef W_MAX_SPEED_FACTOR
#define W_MAX_SPEED_FACTOR 21          //Higher number, higher speed
#endif
#ifndef W_MAX_WAVE_SPEED
#define W_MAX_WAVE_SPEED 2          //Higher number, higher speed
#endif
#ifndef W_BASE_SPEED
#define W_BASE_SPEED 60          //Higher number, higher speed
#endif

#ifndef W_MIN_WIDTH
#define W_MIN_WIDTH 2           //Higher number, smaller waves
#endif

#ifndef W_MAX_WIDTH 
#ifndef W_WIDTH_FACTOR //Higher number, smaller waves
#define W_WIDTH_FACTOR 2
#endif
#define W_MAX_WIDTH ((unsigned int)NUM_LEDS / W_WIDTH_FACTOR)
#endif

#ifndef W_MIN_ALPHA
#define W_MIN_ALPHA  60
#endif
#ifndef W_MAX_ALPHA
#define W_MAX_ALPHA  101
#endif
#ifndef W_INTENSITY
#define W_INTENSITY 255
#endif

#define GRAD_NUM(x) (((255/Parameters.gradientCount)*x))

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
//    0,0,0,0, // black
    0,171,0,85, // pink
//    255,255,165,0// orange
    //
    255,8,232,243 // teal
//  128,   255,255,255, // white
//  192,25,25,25, // black
}; 
/*

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,   255,255,255, // white
  85,171,     85,  0,   //orange
  170,0,0,0, // black
  255,171,0,85, // pink
}; 
DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  GRAD_NUM(0),0,0,0, // black
  GRAD_NUM(1),171,     85,  0,   //orange
  GRAD_NUM(2),0,0,0, // black
  GRAD_NUM(3),255,255,255, //white
  GRAD_NUM(4),0,0,0, // black
  GRAD_NUM(5),171,0,85 // pink
};*/ 


#define _CALC_MIN_WIDTH (max((unsigned int)W_MIN_WIDTH,1u))
LedParameters Parameters = {
  BRIGHTNESS,
  FRAMES_PER_SECOND,
  GRADIENT_COUNT,
  W_MIN_SPEED_FACTOR,
  W_MAX_SPEED_FACTOR,
  W_MAX_WAVE_SPEED,
  W_BASE_SPEED,
  _CALC_MIN_WIDTH,
  max(W_MAX_WIDTH, _CALC_MIN_WIDTH+1),
  TTL,
  W_MIN_ALPHA,
  W_MAX_ALPHA,
  W_INTENSITY,
  heatmap_gp,
  W_DEFAULT_MODE
};
