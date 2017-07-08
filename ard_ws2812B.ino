#include <FastLED.h>

#define LED1_PIN     5
#define LED2_PIN     6
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define NUM_LEDS    120
#define NUM_LEDS_PER_STRIP 60

#define BRIGHTNESS  200
#define FRAMES_PER_SECOND 60

bool gReverseDirection = false;

CRGB leds[NUM_LEDS];

CRGBPalette16 gPal;

void setup() {
  delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, LED1_PIN, COLOR_ORDER>(leds, 0, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<CHIPSET, LED2_PIN, COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
//  FastLED.setBrightness( BRIGHTNESS );
}

void loop()
{
  police();
 // ticker(0,120,CRGB::Green);
 
}

//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120


void Fire2012WithPalette()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}

void stripColor(CRGB color){
   for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      //byte colorindex = scale8( heat[j], 240);
      leds[j] = color;  
    }
    FastLED.show();
}

void police(){
    for (int i=0;i<3;i++){
     parallelTicker(CRGB::Blue,CRGB::Red,10);
    }
    delay(50);
   for (int i=0;i<3;i++){
     twoColorBlink(CRGB::Blue,CRGB::Red,50);
    }
   for (int i=0; i<3;i++){
    roundtrip(0,60,CRGB::Blue,15);
    roundtrip(60,120,CRGB::Red,15);
   }
   
}

void blink_strip(CRGB color, int delay_time){
  stripColor(color);
  delay(delay_time);
}

void twoColorBlink(CRGB color1, CRGB color2, int delay_time){
  for (int i=0; i<NUM_LEDS; i++){
    leds[i]=color1;
    leds[NUM_LEDS-1-i]=color2; 
  }
  FastLED.show();
    delay(delay_time/2);
    blink_strip(CRGB::Black,delay_time/2);
}

void ticker(int led_start, int led_end, CRGB color, int delay_time){
  for (int i=led_start; i<led_end; i++){
    leds[i]=color;
    FastLED.show();
    leds[i]=CRGB::Black;
    delay(delay_time);    
  }  
}

void parallelTicker(CRGB color1, CRGB color2, int delay_time){
    for (int i=0; i<60; i++){
    leds[i]=color1;
    leds[NUM_LEDS-i-1]=color2;
    FastLED.show();
    leds[i]=leds[NUM_LEDS-i-1]=CRGB::Black;
    delay(delay_time);    
  }  
}

void roundtrip(int from, int to, CRGB color, int delay_time){
  for (int i=from; i<to; i++){
    leds [i]=color;
    FastLED.show();
    leds[i]=CRGB::Black;
    delay(delay_time);
  }
  for (int i=to; i>=from; i--){
    leds [i]=color;
    FastLED.show();
    leds[i]=CRGB::Black;
    delay(delay_time);
  }
}

