# leaningBookshelvesLight1
LED leaning bookshelves light, by Thurstan

Repository is an Arduino project

### Equipment
#### Hardware
- Arduino Mini Pro board
  * 5v
  * ATmega328
  * 16 MHz clock speed
  * 1K EEPROM
- WS2812B addressable LED strips
  * pixel order = GRB
- DS3231 RTC (realtime clock) with AT24C32 memory backpack
  * 5v
  * I2C

#### Software
- Arduino 
- EEPROM libary
- Bounce library
- CapacitiveSensor library  https://github.com/PaulStoffregen/CapacitiveSensor
- FastLED library  https://github.com/FastLED/FastLED
- DS3231_Simple library  https://github.com/sleemanj/DS3231_Simple
  * DS3231 I2C address = 0x68
  * AT24C32 I2C address = 0x57

### Licence
- Written by MTS Standish (Thurstan|mattKsp) 2016
- Released under GNU GPLv3 (see LICENCE file)

This software is provided on an "AS-IS-BASIS"

### Reference
https://github.com/FastLED/FastLED/wiki/Pixel-reference <br> http://playground.arduino.cc/Main/CapacitiveSensor <br> http://www.timeanddate.com/sun/uk/york <br> 
 
