# smartStrip

## howto
* setup pins
* setup LED count and LED type
* do magic

## What Arduino can read from serial?
Send data in format ```#0:11#1:22#2:33@```
Where `#0:11` - first variable (in enum you can see, that is LED_MODE), `#1:22` - second, etc. `@` - symbol of the end of string.
More detail about variable:
* `#` - start of the variable
* `1` - variable name (using numbers for enum)
* `:` - equal sign
* `22` - variable value