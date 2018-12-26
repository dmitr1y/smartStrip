#include "FastLED.h"
#include "SoftwareSerial.h"

// ---------------- music ----------------
#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht

#include <FHT.h>

#define inputPin A5

#define qsubd(x, b) ((x>b)?wavebright:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b) ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0.

#define wavebright 128                                        // qsubd result will be this value if subtraction is >0.

uint8_t hueinc = 0;                                               // A hue increment value to make it rotate a bit.
uint8_t micmult = 25;
uint8_t fadetime = 900;
uint8_t noiseval = 30;   
// ---------------- music ----------------


#define NUM_LEDS 98 
CRGB leds[NUM_LEDS];
#define PIN 8 

SoftwareSerial BlueTooth(10, 11); // TX, RX for BT

// dynamically changeable params
unsigned short int ledBrightness = 150; // максимальная яркость (0 - 255)
unsigned short int ledMode = 1;
unsigned short int ledSpeed = 50;

enum BT_DATA { 
  LED_MODE = 0, 
  LED_BRIGHTNESS=1, 
  LED_COLOR=2, 
  LED_SPEED = 3 
};

void setup()
{
  LEDS.addLeds<WS2812B, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  LEDS.setBrightness(ledBrightness);
  set_max_power_in_volts_and_milliamps(5, 4000); 
// init
  setAll(0,0,0);
  showStrip();

  BlueTooth.begin(38400);
  Serial.begin(9600); // открыть порт для связи
}

// *** REPLACE FROM HERE ***
void loop() { 
  if (!checkInput()) // пока ничего не прислали
  {
    ledMods();
  }
  else{
    recieveData();
  }
}

void myDelay(unsigned short int mSec) {
  unsigned short int x=0;

  while(!checkInput() && (x< mSec)){
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
    CylonBounce(0xff, 0, 0, 4, ledSpeed, 50);
    break;
  case 5:
    FadeInOut(0xff, 0x00, 0x00); // red
    FadeInOut(0xff, 0xff, 0xff); // white 
    FadeInOut(0x00, 0x00, 0xff); // blue
    break;
  case 6:
    RGBLoop(ledSpeed);
    break;
  case 7:
    Fire(55,120,ledSpeed);
    break;
  case 8:
    HalloweenEyes(0xff, 0x00, 0x00, 
                1, 4, 
                true, random(5,50), random(50,150), 
                random(1000, 10000));
    break;
  case 9:
    NewKITT(0xff, 0, 0, 8, ledSpeed, 50);
    break;
  case 10:
    rainbowCycle(ledSpeed);
    break;
  case 11:
    TwinkleRandom(20, ledSpeed, false);
    break;
  case 12:
    RunningLights(0xff,0xff,0x00, ledSpeed);
    break;
  case 13:
    SnowSparkle(0x10, 0x10, 0x10, 20, random(100,1000));
    break;
  case 14:
    Sparkle(0xff, 0xff, 0xff, ledSpeed);
    break;
  case 15:
    Strobe(0xff, 0xff, 0xff, 10, 50, ledSpeed);
    break;
  case 16:
    theaterChaseRainbow(ledSpeed);
    break;
  case 17:
    theaterChase(0xff,0,0,ledSpeed);
    break;
  case 18:
    Twinkle(0xff, 0, 0, 10, ledSpeed, false);
    break;
  case 19:
    rgb_rotation(ledSpeed);
    break;
  case 20:
    ems_lightsStrobe(ledSpeed);
    break;
  case 21:
    music();
    break;
  default:
    Serial.println("UNDEFINDED MODE "+String(ledMode));
    setAll(0,0,0);
    showStrip();
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

      // Serial.println("varName: "+ String(varName)+" varVal: "+String(varVal));

      switch (varName) {
          case LED_MODE:
            ledMode=varVal;
            Serial.print("cur MODE: ");
            Serial.println(ledMode);            
            break;

          case LED_BRIGHTNESS:
            ledBrightness=varVal;
            LEDS.setBrightness(ledBrightness); // ограничить максимальную яркость
            showStrip();
            Serial.print("cur BRIGHTNESS: ");
            Serial.println(ledBrightness);       
            break;

          case LED_COLOR:
            break;

          case LED_SPEED:
            ledSpeed=varVal;
            Serial.print("cur SPEED: ");
            Serial.println(ledSpeed);    
            break;

          default:
            break;
      }
      parsed = strtok(NULL, "#:");
   }
   ledMods();
}

bool checkInput(){
   if(BlueTooth.available() > 4 || Serial.available() > 4){
    return true;
  }

  return false;
}

bool recieveData() {
  if(checkInput()){
    Serial.println("Data recieved!");

    String serialReaded = "";

    bool isBT = false;
    if(BlueTooth.available() > 4){
      serialReaded = BlueTooth.readStringUntil('@'); // Until CR (Carriage Return)
      isBT= true;
    } else if(Serial.available() > 4){
      serialReaded = Serial.readStringUntil('@'); // Until CR (Carriage Return)
    }
        
    char *buf = (char *)malloc(sizeof(char) * serialReaded.length());
    serialReaded.toCharArray(buf, serialReaded.length() + 1);
    parseData(buf); // parse data
    
    if(isBT){
      BlueTooth.flush();        
    }
    else{
      Serial.flush();
    }
    
    free(buf);
    // ledMods();
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