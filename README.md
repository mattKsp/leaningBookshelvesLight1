# leaningBookshelvesLight1
LED leaning bookshelves light, by Thurstan

Repository is an Arduino project

### Equipment
#### Hardware
- ~~Arduino Mini Pro board, 5v, ATmega328, 16 MHz, 1K EEPROM~~
- WeMos D1 (R2 &) mini
  * 80 MHz 
  * 115200 baud 
  * 4M (3M SPIFFS)
- WS2812B addressable LED strips
  * pixel order = GRB
- DS3231 RTC (realtime clock) with AT24C32 memory backpack
  * 5v
  * I2C
- Grove - 12 Key Capacitive I2C Touch Sensor V2 (MPR121)
  * 3.3v and 5v
  * I2C

#### Software
- Arduino 
- EEPROM libary
- Bounce library
- CapacitiveSensor library  https://github.com/PaulStoffregen/CapacitiveSensor
- FastLED library  https://github.com/FastLED/FastLED
- DS3231_Simple library  https://github.com/sleemanj/DS3231_Simple
  * DS3231 I2C address = 0x68 (104)
  * AT24C32 I2C address = 0x57 (87)
- Grove touch sensor V2 library  https://github.com/linux-downey/Grove_touch_sensor_MPR121
  * I2C address = 0x5B (91), 0x5C (92) or 0x5D (93)

### Licence
- Written by MTS Standish (Thurstan|mattKsp) 2016
- Released under GNU GPLv3 (see LICENCE file)

This software is provided on an "AS-IS-BASIS"

### Reference
https://github.com/FastLED/FastLED/wiki/Pixel-reference <br> http://playground.arduino.cc/Main/CapacitiveSensor <br> http://www.timeanddate.com/sun/uk/york <br> http://wiki.seeedstudio.com/Grove-12_Key_Capacitive_I2C_Touch_Sensor_V2-MPR121/ <br> 
 
