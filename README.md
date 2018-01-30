# smartStrip

Many operating modes for the LED strip WS2811, WS2812, WS2812b. By default, the control is via Bluetooth.

## Howto
* setup pins
* setup LED count and LED type
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
999: break; // pazooka
2: rainbow_fade (); break; // smooth change of colors of all tape
3: rainbow_loop (); break; // a twirling rainbow
4: random_burst (); break; // random color change
5: color_bounce (); break; // running LED
6: color_bounceFADE (); break; // running locomotive of light-emitting diodes
7: ems_lightsONE (); break; // rotate red and blue
8: ems_lightsALL (); break; // rotates half red and half blue
9: flicker (); break; // random stroboscope
10: pulse_one_color_all (); break; // pulsation in one color
11: pulse_one_color_all_rev (); break; // pulsation with a change of colors
12: fade_vertical (); break; // smooth brightness change vertically (for a ring)
13: rule30 (); break; // the madness of the red LEDs
14: random_march (); break; // frenzy of random colors
15: rwb_march (); break; // white blue red run in a circle (PATRIOTISM!)
16: radiation (); break; // the radiation icon is pulsating
17: color_loop_vardelay (); break; // the red LED runs in a circle
18: white_temps (); break; // white blue gradient (?)
19: sin_bright_wave (); break; / / also some crap
20: pop_horizontal (); break; // red flashes go down
21: quad_bright_curve (); break; // crescent moon
22: flame (); break; // flame effect
23: rainbow_vertical (); break; // a rainbow in the vertical plane (ring)
24: pacman (); break; // pakman
25: random_color_pop (); break; // the frenzy of random flashes
26: ems_lightsSTROBE (); break; // police flasher
27: rgb_propeller (); break; // RGB propeller
28: kitt (); break; // random flashes of red in the vertical plane
29: matrix (); break; // the greens run around in circles by accident
30: new_rainbow_loop (); break; // steep smooth rotating rainbow
31: strip_march_ccw (); break; // even broke
32: strip_march_cw (); break; // even broke
33: colorWipe (0x00, 0xff, 0x00, thisdelay); colorWipe (0x00, 0x00, 0x00, thisdelay); break; // smooth filling with color
34: CylonBounce (0xff, 0, 0, 4, 10, thisdelay); break; // running LEDs
35: Fire (55, 120, thisdelay); break; // Linear fire
36: NewKITT (0xff, 0, 0, 8, 10, thisdelay); break; // run the sectors of the circle (does not work)
37: rainbowCycle (thisdelay); break; // very smooth rotating rainbow
38: TwinkleRandom (20, thisdelay, 1); break; // random multicolored inclusions (1 - all dance, 0 - random 1 diode)
39: RunningLights (0xff, 0xff, 0x00, thisdelay); break; // running lights
40: Sparkle (0xff, 0xff, 0xff, thisdelay); break; // random flashes of white color
41: SnowSparkle (0x10, 0x10, 0x10, thisdelay, random (100, 1000)); break; // random flash of white color on a white background
42: theaterChase (0xff, 0, 0, thisdelay); break; // running every 3 (NUMBER OF LEDS MUST BE ODD)
43: theaterChaseRainbow (thisdelay); break; // running every 3 rainbows (NUMBER OF LEDS SHOULD BE BACK 3)
44: Strobe (0xff, 0xff, 0xff, 10, thisdelay, 1000); break; // stroboscope
45: BouncingBalls (0xff, 0, 0, 3); break; // jumping balls
46: BouncingColoredBalls (3, ballColors); break; // jumping colored balls
888: demo_modeA (); break; // a long demo
889: demo_modeB (); break; // short demo
```