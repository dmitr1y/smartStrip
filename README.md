# smartStrip

Many operating modes for the LED strip WS2811, WS2812, WS2812b. By default, the control is via Bluetooth and Serial.

## Howto
* setup pin
* setup LED count and LED type
* setup Bluetooth baudrate
* do magic

## What Arduino can read from serial?
Send data in format ```#0:11#1:22#2:33@```
Where `#0:11` - first variable (in enum you can see, that is LED_MODE), `#1:22` - second, etc. `@` - symbol of the end of string.

## More detail about variable:
* `#` - start of the variable
* `1` - variable name (using numbers for enum)
* `:` - equal sign
* `22` - variable value

## Modes

```
ALL_OFF(999, "Выключить"), // all leds is off
BOUNCING_COLORED_BALLS(1, "Прыгающие цветные шарики"),
BOUNCING_BALLS(2, "Прыгающие шарики"),
COLOR_WIPE(3, "Заполнение цветом"),
CYLON_BOUNCE(4, "Cylon Bounce"),
FADE_RWB(5, "FadeInOut RED WHITE BLUE"),
RGB_LOOP(6, "Плавная смена цветов"),
FIRE(7, "Эффект огня"),
HALLOWEEN_EYES(8, "Halloween Eyes"),
NEW_KITT(9, "NEW_KITT"),
RAINBOW_CYCLE(10, "Вращающаяся радуга"),
TWINKLE_RANDOM(11, "TWINKLE_RANDOM"),
RUNNING_LIGHTS(12, "RUNNING_LIGHTS"),
SNOW_SPARKLE(13, "SNOW_SPARKLE"),
SPARKLE(14, "SPARKLE"),
STROBE(15, "STROBE"),
THEATER_CHASE_RAINBOW(16, "THEATER_CHASE_RAINBOW"),
THEATER_CHASE(17, "THEATER_CHASE"),
TWINKLE(18, "TWINKLE"),
RGB_ROTATION(19, "RGB_ROTATION"),
EMS_LIGHTS_STROBE(20, "EMS_LIGHTS_STROBE");
```

## Android app
Together with this firmware, you can use the [application](https://github.com/dmitr1y/LongboardLighting) for convenient control of the LED tape.