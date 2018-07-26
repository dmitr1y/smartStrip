#include <FastLED.h>
#include <SoftwareSerial.h>

// #define LED_COUNT 10        // число светодиодов в кольце/ленте
// #define LED_COUNT 121        // число светодиодов в кольце/ленте
#define LED_COUNT 105 // число светодиодов в кольце/ленте
#define LED_DT 8      // пин, куда подключен DIN ленты

SoftwareSerial BlueTooth(4, 5); // TX, RX for BT

volatile unsigned short int max_bright = 150; // максимальная яркость (0 - 255)
int ledMode = -1;

// цвета мячиков для режима
byte ballColors[3][3] = {{0xff, 0, 0}, {0xff, 0xff, 0xff}, {0, 0, 0xff}};

// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------
unsigned short int BOTTOM_INDEX = 0; // светодиод начала отсчёта
unsigned short int TOP_INDEX = (unsigned short int)(LED_COUNT / 2);
unsigned short int EVENODD = LED_COUNT % 2;
struct CRGB leds[LED_COUNT];
volatile unsigned short int
    ledsX[LED_COUNT][3]; //-ARRAY FOR COPYING WHATS IN THE LED STRIP
                         // CURRENTLY (FOR CELL-AUTOMATA, MARCH, ETC)

unsigned short int thisdelay = 20; //-FX LOOPS DELAY VAR
unsigned short int thisstep = 10;  //-FX LOOPS DELAY VAR
unsigned short int thishue = 0;    //-FX LOOPS DELAY VAR

unsigned short int idex = 0;            //-LED INDEX (0 to LED_COUNT-1
unsigned short int ihue = 0;            //-HUE (0-255)
unsigned short int ibright = 0;         //-BRIGHTNESS (0-255)
unsigned short int isat = 0;            //-SATURATION (0-255)
unsigned short int bouncedirection = 0; //-SWITCH FOR COLOR BOUNCE (0-1)
float tcount = 0.0;                     //-INC VAR FOR SIN LOOPS
unsigned short int lcount = 0;          //-ANOTHER COUNTING VAR

// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------

enum BT_DATA { LED_MODE = 0, LED_BRIGHTNESS, LED_COLOR, SYS_VOLT };

void setup() {
  // config of Serial
  Serial.begin(9600); // открыть порт для связи
  BlueTooth.begin(9600);

  // config of led strip
  LEDS.setBrightness(max_bright); // ограничить максимальную яркость
  LEDS.addLeds<WS2812B, LED_DT, GRB>(
      leds, LED_COUNT); // настрйоки для нашей ленты (ленты на WS2811, WS2812,
                        // WS2812B)
  one_color_all(0, 0, 0); // погасить все светодиоды
  // pinMode(7, INPUT);     //assigning the input port to Vmeter

  // config of interrupts
  // attachInterrupt(0, recieveData, HIGH);
  // pinMode(6, OUTPUT); // reset for RS-trigger
  // digitalWrite(6, LOW);
  // pinMode(2, INPUT); // interrupt pins

  LEDS.show(); // отослать команду
}

void loop() {
	Serial.println("------> loop");
  // digitalWrite(6, LOW);
  while (BlueTooth.available()) // если что то прислали
  {
    Serial.println("Data recieved!");
    recieveData();
    // delay(1);
  }
  while (BlueTooth.available()<4)
  	ledMods();
}

void ledMods() {
  Serial.print("cur ledMode: ");
  Serial.println(ledMode);
  switch (ledMode) {
  case 999:
    break;
  case -1:
    one_color_all(0, 0, 0);
    LEDS.show();
    break; //---ALL OFF                       // пауза
  case 2:
    thisdelay = 20;
    rainbow_fade();
    break; // плавная смена цветов всей ленты
  case 4:
    thisdelay = 20;
    thisstep = 10;
    random_burst();
    break; // случайная смена цветов
  case 5:
    thisdelay = 20;
    thishue = 0;
    color_bounce();
    break; // бегающий светодиод
  case 6:
    thisdelay = 40;
    thishue = 0;
    color_bounceFADE();
    break; // бегающий паровозик светодиодов
  case 7:
    thisdelay = 40;
    thishue = 0;
    ems_lightsONE();
    break; // вращаются красный и синий
  case 8:
    thisdelay = 40;
    thishue = 0;
    ems_lightsALL();
    break; // вращается половина красных и половина синих
  case 10:
    thisdelay = 15;
    thishue = 0;
    pulse_one_color_all();
    break; // пульсация одним цветом
  case 11:
    thisdelay = 15;
    thishue = 0;
    pulse_one_color_all_rev();
    break; // пульсация со сменой цветов
  case 16:
    thisdelay = 60;
    thishue = 95;
    radiation();
    break; // пульсирует значок радиации
  case 18:
    white_temps();
    break; // бело синий градиент (?)
  case 20:
    thisdelay = 100;
    thishue = 0;
    pop_horizontal();
    break; // красные вспышки спускаются вниз
  case 21:
    thisdelay = 100;
    thishue = 180;
    quad_bright_curve();
    break; // полумесяц
  case 22:
    flame();
    break; // эффект пламени
  case 25:
    thisdelay = 35;
    random_color_pop();
    break; // безумие случайных вспышек
  case 26:
    thisdelay = 25;
    thishue = 0;
    ems_lightsSTROBE();
    break; // полицейская мигалка
  case 27:
    thisdelay = 25;
    thishue = 0;
    rgb_propeller();
    break; // RGB пропеллер
  case 29:
    thisdelay = 50;
    thishue = 95;
    matrix();
    break; // зелёненькие бегают по кругу случайно
  case 30:
    thisdelay = 5;
    new_rainbow_loop();
    break; // крутая плавная вращающаяся радуга
  case 33:
    thisdelay = 50;
    colorWipe(0x00, 0xff, 0x00, thisdelay);
    colorWipe(0x00, 0x00, 0x00, thisdelay);
    break; // плавное заполнение цветом
  case 34:
    thisdelay = 50;
    CylonBounce(0xff, 0, 0, 4, 10, thisdelay);
    break; // бегающие светодиоды
  case 35:
    thisdelay = 15;
    Fire(55, 120, thisdelay);
    break; // линейный огонь
  case 37:
    thisdelay = 20;
    rainbowCycle(thisdelay);
    break; // очень плавная вращающаяся радуга
  case 38:
    thisdelay = 10;
    TwinkleRandom(20, thisdelay, 1);
    break; // случайные разноцветные включения (1 - танцуют все, 0 -
           // случайный 1 диод)
  case 39:
    thisdelay = 50;
    RunningLights(0xff, 0xff, 0x00, thisdelay);
    break; // бегущие огни
  case 41:
    thisdelay = 20;
    SnowSparkle(0x10, 0x10, 0x10, thisdelay, random(100, 1000));
    break; // случайные вспышки белого цвета на белом фоне
  case 42:
    thisdelay = 50;
    theaterChase(0xff, 0, 0, thisdelay);
    break; // бегущие каждые 3 (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ НЕЧЁТНОЕ)
  case 43:
    thisdelay = 50;
    theaterChaseRainbow(thisdelay);
    break; // бегущие каждые 3 радуга (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ КРАТНО
           // 3)
  case 44:
    thisdelay = 100;
    Strobe(0xff, 0xff, 0xff, 10, thisdelay, 1000);
    break; // стробоскоп
  case 45:
    BouncingBalls(0xff, 0, 0, 3);
    break; // прыгающие мячики
  case 46:
    BouncingColoredBalls(3, ballColors);
    break; // прыгающие мячики цветные
  case 889:
    demo_modeB();
    break; // короткое демо
  default:
    break;
  }
  bouncedirection = 0;
}

void recieveData() {
  Serial.println("recieveData");
  String serialReaded =
      BlueTooth.readStringUntil('@'); // Until CR (Carriage Return)
  char *buf = (char *)malloc(sizeof(char) * serialReaded.length());
  serialReaded.toCharArray(buf, serialReaded.length() + 1);
  parseData(buf); // parse data
  BlueTooth.flush();
  free(buf);
  sendData();
  // digitalWrite(6, HIGH);
}

float voltMeter() {
  // Voltmeter
  float R1 = 100000.00; // resistance of R1 (100K)
  float R2 = 10000.00;  // resistance of R2 (10K)
  int val = 0;
  val = analogRead(7); // reads the analog input
  float Vout =
      (val * 5.00) /
      1024.00; // formula for calculating voltage out i.e. V+, here 5.00
  float Vin =
      Vout / (R2 / (R1 + R2)); // formula for calculating voltage in i.e. GND
  if (Vin < 0.09)              // condition
    Vin = 0.00;                // statement to quash undesired reading !
  return Vin;
}

void sendData() {
  String data = "";
  data += "#3:" + String(voltMeter()) + "@";
  BlueTooth.println(data);
}

void parseData(char *data) {
  Serial.print("Parsing data: ");
  Serial.println(data);
  char *parsed = strtok(data, "#");
  char **varArr = (char **)malloc(sizeof(char));
  short int arraySize=0;
  while (parsed!=NULL){
    varArr=(char**)realloc(varArr,sizeof(char*)*(arraySize+1));
    varArr[arraySize]=(char*)malloc(sizeof(char*));
    varArr[arraySize]=parsed;
    arraySize++;
    parsed=strtok(NULL, "#");
  }
  for (short int j = 0; j < arraySize; ++j) {
      char *tmp = strtok(varArr[j],":");
      short int varName = atoi(tmp);
      tmp = strtok(NULL,":");
      short int varVal = atoi(tmp);    
      switch (varName){
          case LED_MODE:
              Serial.print("LED_MODE = ");
              ledMode=varVal;
              Serial.println(ledMode);
              break;
          case LED_BRIGHTNESS:
              max_bright=varVal;
              LEDS.setBrightness(max_bright);
              break;
          case LED_COLOR:
              break;
          case SYS_VOLT:  
              break;
          default:
              break;         
      }
  }
  if (varArr!=NULL)
  {
  	Serial.println("varArr isn't NULL!");
  	for (short int i = 0; i < arraySize; ++i)
  		free(varArr[i]);
  	Serial.println("       deleting header...");
    free(varArr);
    Serial.println("       	... OK.");
  }
}