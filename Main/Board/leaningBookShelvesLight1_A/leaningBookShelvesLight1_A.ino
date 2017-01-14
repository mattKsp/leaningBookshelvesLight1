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
//..insert UV sensor return pins here.. (may need to upgrade board, or expand with scanning chip)
const int _capSenseSendPin = 6;             //capacitive touch sensor (send)
const int _capSense0Pin = 7;                //on/off (receive) - note: all other touch sensors will trigger 'on' if 'off', aswell as this pin..
const int _capSense1Pin = 8;                //mode - capacitive touch sensor (receive)
const int _capSense2Pin = 9;                //sub-mode - capacitive touch sensor (receive) - not in use yet
const int _capSense3Pin = 10;               //volume up - capacitive touch sensor (receive)
const int _capSense4Pin = 11;               //volume down - capacitive touch sensor (receive)
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
const String _progVers = "0.24";             //cloned from deskLight1_A v0.23
//const int _mainLoopDelay = 0;               //just in case  - using FastLED.delay instead..
boolean _firstTimeSetupDone = false;        //starts false //this is mainly to catch an interrupt trigger that happens during setup, but is usefull for other things
volatile boolean _onOff = false;            //this should init false, then get activated by input - on/off true/false

/*----------------------------modes----------------------------*/
const int _modeNum = 9;
const int _modePresetSlotNum = 7;
int _modePreset[_modePresetSlotNum] = { 0, 2, 3, 4, 5, 7, 8 }; //test basic, tap bt to cycle around 3 mode slots   //expand to array or struct later for more presets
volatile int _modeCur = 0;                  //current mode in use - this is not the var you are looking for.. try _modePresetSlotCur
int _modePresetSlotCur = 0;                 //the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput

/*----------------------------touch sensors----------------------------*/
CapacitiveSensor _touch0 = CapacitiveSensor(_capSenseSendPin,_capSense0Pin);  //on/off
CapacitiveSensor _touch1 = CapacitiveSensor(_capSenseSendPin,_capSense1Pin);  //mode
//CapacitiveSensor _touch2 = CapacitiveSensor(_capSenseSendPin,_capSense2Pin);  //sub-mode
//CapacitiveSensor _touch3 = CapacitiveSensor(_capSenseSendPin,_capSense3Pin);  //brightness up
//CapacitiveSensor _touch4 = CapacitiveSensor(_capSenseSendPin,_capSense4Pin);  //brightness down

byte _touchSensorRes = 20;  //sample/sensor resolution - higher is better but slower to read
long _touchSensorThreshold = 100;  //??? no idea - start at 100 and test  //unsigned long   //1 for all at the moment

//1,000 microseconds in a millisecond and 1,000,000 microseconds in a second
//eg. 6000 millisecond = 0.6 milliseconds = 0.0006 seconds
//micros() may be too small, might end up using millis()
const long _touchDeBounceInterval = 500;                            //interval to de-bounce in milliseconds    //const int 
long _touchPrevMillis[5] = { 0, 0, 0, 0, 0 };                          //how long between 'bounces' //unsigned long
boolean _touchToggled[5] = { false, false, false, false, false };

/*----------------------------LED----------------------------*/
/*
 * the bookshelf has 3 main uprights, with 3 segmented LED strips running up each.
 * there are 10 shelves, therefore 10 segments.
 * 2 of the uprights are full height (2.4m, 10 shelves), and the 3rd is shorter (0.8m, 3 shelves).
 * the 3rd strip is still addressed as a full clone, even though most of it isn't there to receive the signals.
 * ..easier this way. there is no return data, so this is just allocating memory and sending signals to nowhere.
 * ledSegment[] is the main setup (!!! needs name change !!!). bottom to top, segmented by shelf location (the shelf height lowers as we go up the bookshelf. bigger books on the bottom shelves, smaller on the top).
 * ..mabye.. ledSegmentVerticalA[]
 * 
 * hmm.. might have to change this structure around and list individual numbers in fixed arrays...
 */
typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;
const int _ledNumOfStrips = 3;                  //3x LED strips (12, 34, 34)
const int _ledNumPerStrip = 35;                 //Xm strip with LEDs
const int _segmentTotal = 10;                   //total segments (shelves) on each strip
const int _ledGlobalBrightness = 255;           //global brightness - use this to cap the brightness
int _ledGlobalBrightnessCur = 255;              //current global brightness - adjust this one!
int _ledBrightnessIncDecAmount = 10;            //the brightness amount to increase or decrease
#define UPDATES_PER_SECOND 120                  //main loop FastLED show delay //100
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

CRGB leds[_ledNumOfStrips][_ledNumPerStrip];    //global RGB array matrix
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
  //delay(_mainLoopDelay);  //using FastLED.delay instead..
}

