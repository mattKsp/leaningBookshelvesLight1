/*
    'leaningBookshelvesLight1_A' by Thurstan. LED strip bookshelves light.
    Copyright (C) 2016  MTS Standish (Thurstan|mattKsp)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be usefull,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    https://github.com/mattKsp/
*/

// Tested on Arduino Pro Mini (5v, ATmega328, 16mhz clock speed, 32KB Flash, 2KB SRAM, 1KB EEPROM)


#define DEBUG 1                             //comment/un-comment
const int _ledDOut0Pin = 2;                 //DOut 0 -> LED strip 0 DIn
const int _ledDOut1Pin = 3;                 //DOut 1 -> LED strip 1 DIn
const int _ledDOut2Pin = 4;                 //DOut 2 -> LED strip 2 DIn
const int _uvSendPin = 5;                   //grouped send for uv emittor diodes - check, this should be digital pin..
const int _capSenseSendPin = 6;             //capacitive touch sensor (send)
const int _capSense0Pin = 7;               //mode - capacitive touch sensor (receive)
const int _capSense1Pin = 8;               //sub-mode - capacitive touch sensor (receive)
const int _capSense2Pin = 9;               //volume up - capacitive touch sensor (receive)
const int _capSense3Pin = 10;               //volume down - capacitive touch sensor (receive)
const int _ledPin = 13;                     //built-in LED

/*----------------------------libraries----------------------------*/
#include <EEPROM.h>                         //a few saved settings
#include <CapacitiveSensor.h>               //capacitive touch sensors
#include <FastLED.h>                        //WS2812B LED strip control and effects

/*----------------------------system----------------------------*/
/* 
 use if defined statements and comment out to disable debug mode 
 this will remove all debug code when compiling rather than just switching off
 for now only use serial when in debug 
*/
const String _progName = "bookShelvesLight1_A";
const String _progVers = "0.21";             //cloned from deskLight1_A v0.23
const int _mainLoopDelay = 0;               //just in case
boolean _firstTimeSetupDone = false;        //starts false //this is mainly to catch an interrupt trigger that happens during setup, but is usefull for other things
volatile boolean _onOff = false;            //this should init false, then get activated by input - on/off true/false

/*----------------------------modes----------------------------*/
const int _modeNum = 9;
const int _modePresetSlotNum = 3;
int _modePreset[_modePresetSlotNum] = { 0, 4, 5 }; //test basic, tap bt to cycle around 3 mode slots   //expand to array or struct later for more presets
volatile int _modeCur = 1;                  //current mode in use - this is not the var you are looking for.. try _modePresetSlotCur
int _modePresetSlotCur = 0;                 //the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput

/*----------------------------touch sensors----------------------------*/
CapacitiveSensor _touch0 = CapacitiveSensor(_capSenseSendPin,_capSense0Pin);  //mode
//CapacitiveSensor _touch1 = CapacitiveSensor(_capSenseSendPin,_capSense1Pin);  //sub-mode
CapacitiveSensor _touch2 = CapacitiveSensor(_capSenseSendPin,_capSense2Pin);  //brightness up
CapacitiveSensor _touch3 = CapacitiveSensor(_capSenseSendPin,_capSense3Pin);  //brightness down
//boolean _touchSensorToggled[3];

/*----------------------------LED----------------------------*/
typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;
const int _ledNumOfStrips = 3;                  //3x LED strips (12, 34, 34)
const int _ledNumPerStrip = 35;                 //Xm strip with  LEDs
//const int _ledNum = 40;                         //TEMP testing - 55 on roll, using 40
const int _segmentTotal = 10;                   //total segments on each strip
const int _ledGlobalBrightness = 255;           //global brightness
#define UPDATES_PER_SECOND 100                  //main loop FastLED show delay
LED_SEGMENT ledSegment[_segmentTotal] = { 
    { 0, 0, 1 }, 
    { 1, 5, 5 }, 
    { 6, 10, 5 },
    { 11, 14, 4 },
    { 15, 18, 4 },
    { 19, 22, 4 },
    { 23, 25, 3 },
    { 25, 27, 3 },
    { 28, 30, 3 },
    { 31, 34, 4 }
};                 
CHSV startColor( 144, 70, 64 );
CHSV endColor( 31, 71, 69 );
CRGB startColor_RGB( 3, 144, 232 );
CRGB endColor_RGB( 249, 180, 1 );

CRGB leds[_ledNumOfStrips][_ledNumPerStrip];    //global RGB array
int _ledState = LOW;                            //use to toggle LOW/HIGH (ledState = !ledState)
#define TEMPERATURE_0 WarmFluorescent
#define TEMPERATURE_1 StandardFluorescent
#define TEMPERATURE_2 CoolWhiteFluorescent
int _colorTempCur = 5;                          //current colour temperature

/*----------------------------MAIN----------------------------*/
void setup() {
  
  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println();
    Serial.print(_progName);
    Serial.print(" ");
    Serial.print(_progVers);
    Serial.println();
    Serial.print("..");
    Serial.println();
  #endif
  
  delay(3000);                              //give the power, LED strip, etc. a couple of secs to stabilise
  setupLEDs();
  //setupUserInputs();                      //touch sensors already setup by library

  #ifdef DEBUG
  //everything done? ok then..
    Serial.print(F("Setup done"));
    Serial.println();
    blinkStatusLED();
  #endif
}

void loop() {
  
  if(_firstTimeSetupDone == false) {
    #ifdef DEBUG
    //
    #endif
    _firstTimeSetupDone = true;           //need this for stuff like setting sunrise, cos it needs the time to have been set
  }

  loopUserInputs();
  loopModes();
  
  FastLED.show();                           //send all the data to the strips
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  //
  delay(_mainLoopDelay);
}

