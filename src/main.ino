#include "FastLED.h"
#define NUM_LEDS 18 
CRGB leds[NUM_LEDS];
#define PIN 8 

unsigned short int max_bright = 150; // максимальная яркость (0 - 255)
short int ledMode = 1;

// for tests rainbow fade
byte initialHue = 0;

enum BT_DATA { LED_MODE = 0, LED_BRIGHTNESS=1, LED_COLOR=2, SYS_VOLT=3 };

void setup()
{

  LEDS.addLeds<WS2812B, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  LEDS.setBrightness(max_bright);

  Serial.begin(9600); // открыть порт для связи
}

// *** REPLACE FROM HERE ***
void loop() { 

  if (!recieveData()) // если что то прислали
  {
    ledMods();
  }

}

void myDelay(int mSec) {
  int x=0;
  while(x< mSec && !recieveData()){
      delay(1);
      x++;
  }
}

void ledMods() {
  // Serial.print("cur ledMode: ");
  // Serial.println(ledMode);
  byte colors[3][3] = { {0xff, 0,0}, 
                        {0xff, 0xff, 0xff}, 
                        {0   , 0   , 0xff} };
  switch (ledMode) {
  case 1:
    BouncingColoredBalls(3, colors);
    break;
  case 2:
    BouncingBalls(0xff,0,0, 3);
    break;
  case 3:
    colorWipe(0x00,0xff,0x00, 50);
    colorWipe(0x00,0x00,0x00, 50);
    break;
  case 4:
    CylonBounce(0xff, 0, 0, 4, 10, 50);
    break;
  case 5:
    FadeInOut(0xff, 0x00, 0x00); // red
    FadeInOut(0xff, 0xff, 0xff); // white 
    FadeInOut(0x00, 0x00, 0xff); // blue
    break;
  case 6:
    RGBLoop();
    break;
  case 7:
    Fire(55,120,15);
    break;
  case 8:
    HalloweenEyes(0xff, 0x00, 0x00, 
                1, 4, 
                true, random(5,50), random(50,150), 
                random(1000, 10000));
    break;
  case 9:
    NewKITT(0xff, 0, 0, 8, 10, 50);
    break;
  case 10:
    rainbowCycle(20);
    break;
  case 11:
    TwinkleRandom(20, 100, false);
    break;
  case 12:
    RunningLights(0xff,0xff,0x00, 50);
    break;
  case 13:
    SnowSparkle(0x10, 0x10, 0x10, 20, random(100,1000));
    break;
  case 14:
    Sparkle(0xff, 0xff, 0xff, 0);
    break;
  case 15:
    Strobe(0xff, 0xff, 0xff, 10, 50, 1000);
    break;
  case 16:
    theaterChaseRainbow(50);
    break;
  case 17:
    theaterChase(0xff,0,0,50);
    break;
  case 18:
    Twinkle(0xff, 0, 0, 10, 100, false);
    break;
  case 19:
    spinningRainbow();
    break;
  default:
    Serial.println("UNDEFINDED MODE "+String(ledMode));
    break;
  }
  
}
// ---> here we define the effect function <---
// *** REPLACE TO HERE ***

void parseData(char *data) {
  Serial.print("Parsing data: ");
  Serial.println(data);
  char *parsed = strtok(data, "#:"); // разбор на подстроки по ключу #
  
  while (parsed != NULL) {
      int varName = atoi(parsed); // перевод первой части (0) в int  
          
      parsed = strtok(NULL, "#:");
      int varVal = atoi(parsed); // перевод второй части (1) в int

      Serial.println("varName: "+ String(varName)+" varVal: "+String(varVal));

      switch (varName) {
          case LED_MODE:
            ledMode=varVal;
            Serial.print("cur ledMode: ");
            Serial.println(ledMode);            
            break;

          case LED_BRIGHTNESS:
            max_bright=varVal;
            LEDS.setBrightness(max_bright); // ограничить максимальную яркость
            // showStrip();
            Serial.print("cur BRIGHTNESS: ");
            Serial.println(max_bright);       
            break;

          default:
            break;
      }
      ledMods();
      parsed = strtok(NULL, "#:");
   }
}


bool recieveData() {
  if(Serial.available() > 4){
    Serial.println("Data recieved!");
    String serialReaded =
        Serial.readStringUntil('@'); // Until CR (Carriage Return)
    char *buf = (char *)malloc(sizeof(char) * serialReaded.length());
    serialReaded.toCharArray(buf, serialReaded.length() + 1);
    parseData(buf); // parse data
    Serial.flush();
    free(buf);
    return true;
  }

  return false;
}

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   LEDS.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}