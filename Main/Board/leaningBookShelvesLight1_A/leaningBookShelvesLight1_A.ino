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
#ifdef DEBUG     
//#define SET_TIME_BY_SERIAL 1              //(needs debug for serial) 'un-comment' to get a serial prompt at startup to set the time. set time. then 'comment' and upload again
#endif

/*----------------------------arduino pins----------------------------*/

/* 
 * Arduino Pro Mini pins:
 * 
 * serial 0,1
 * interrupts 2,3
 * PWM 3,5,6,9,10,11
 * SPI 10(SS),11(MOSI),13(SCK)
 * I2C A4(SDA),A5(SCL) 
 */

const int _DS3231interruptPin = 2;          //the alarm return from the DS3231 (INT/SQW pin on chip)
const int _ledDOut0Pin = 3;                 //DOut 0 -> LED strip 0 DIn   - right (short)
const int _ledDOut1Pin = 4;                 //DOut 1 -> LED strip 1 DIn   - middle (long)
const int _ledDOut2Pin = 5;                 //DOut 2 -> LED strip 2 DIn   - left (long)
const int _uvSendPin = 6;                   //grouped send for uv emittor diodes - check, this should be digital pin..
//..insert UV sensor return pins here.. (may need to upgrade board, or expand with scanning chip)
const int _capSenseSendPin = 7;             //capacitive touch sensor (send)
const int _capSense0Pin = 8;                //on/off (receive) - note: all other touch sensors will trigger 'on' if 'off', aswell as this pin..
const int _capSense1Pin = 9;                //mode - capacitive touch sensor (receive)
const int _capSense2Pin = 10;                //sub-mode - capacitive touch sensor (receive) - not in use yet
const int _capSense3Pin = 11;               //volume up - capacitive touch sensor (receive)
const int _capSense4Pin = 12;               //volume down - capacitive touch sensor (receive)
const int _ledPin = 13;                     //built-in LED

/*----------------------------libraries----------------------------*/
#include <EEPROM.h>                         //a few saved settings
#include <DS3231_Simple.h>                //DS3231 realtime clock (with AT24C32 memory backback)
//#include <Bounce2.h>                      //buttons with de-bounce
#include <CapacitiveSensor.h>               //capacitive touch sensors
#include <FastLED.h>                        //WS2812B LED strip control and effects
#include <MT_BlinkStatusLED.h>

/*----------------------------system----------------------------*/
/* 
 use if defined statements and comment out to disable debug mode 
 this will remove all debug code when compiling rather than just switching off
 for now only use serial when in debug 
*/
const String _progName = "bookShelvesLight1_A";
const String _progVers = "0.25";             //moved DS3231 clock from deskLight1_A
//const int _mainLoopDelay = 0;               //just in case  - using FastLED.delay instead..
boolean _firstTimeSetupDone = false;        //starts false //this is mainly to catch an interrupt trigger that happens during setup, but is usefull for other things
volatile boolean _onOff = false;            //this should init false, then get activated by input - on/off true/false
#ifdef DEBUG
String _inputString = "";                   // a string to hold incoming data
boolean stringComplete = false;             // whether the string is complete
#endif

/*----------------------------modes----------------------------*/
const int _modeNum = 9;
const int _modePresetSlotNum = 7;
int _modePreset[_modePresetSlotNum] = { 0, 2, 3, 4, 5, 7, 8 }; //test basic, tap bt to cycle around 3 mode slots   //expand to array or struct later for more presets
volatile int _modeCur = 0;                  //current mode in use - this is not the var you are looking for.. try _modePresetSlotCur
int _modePresetSlotCur = 0;                 //the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput

/*-----------------RTC (DS3231 and AT24C32) on I2C------------------*/
DS3231_Simple RTC;                          //init realtime clock
#define DS3231_I2C_ADDRESS 0x68             //default - only used for interrupt kick
//boolean _sunRiseEnabled = false;
//volatile boolean _sunRiseTriggered = false;
//boolean _sunSetEnabled = false;
//volatile boolean _sunSetTriggered = false;
//volatile boolean _sunRiseSetTriggered = false;
int _sunRiseStateCur = 0;                   //current sunrise internal state (beginning, rise, end)
int _sunSetStateCur = 0;                    //current sunset internal state (beginning, fall, end)

/*----------------------------buttons----------------------------*/

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
  
  RTC.begin();                              //DS3231 RealTimeClock begin..
  //RTC.formatEEPROM();
  setupInterrupts();
  
  delay(3000);                              //give the power, LED strip, etc. a couple of secs to stabilise
  setupLEDs();
  //setupUserInputs();                      //touch sensors already setup by library

  #ifdef DEBUG
    #ifdef SET_TIME_BY_SERIAL
      RTC.promptForTimeAndDate(Serial);
    #endif
    Serial.println(F("Current date/time = "));
    RTC.printTo(Serial);
  #endif
  
  //setSunRise(9, 30);      //TEMP
  DS3231kickInterrupt();  //TEMP util
  
  #ifdef DEBUG
  //everything done? ok then..
    Serial.print(F("Setup done"));
    Serial.println();
  #endif
  blinkStatusLED();
}

void loop() {
  
  if(_firstTimeSetupDone == false) {
    #ifdef DEBUG
    //
    #endif
    _firstTimeSetupDone = true;           //need this for stuff like setting sunrise, cos it needs the time to have been set
  }

  #ifdef DEBUG
    if (stringComplete) {
      Serial.println(_inputString);
      _inputString = "";                    //clear the string:
      stringComplete = false;
    }
//    // To check the alarms we just ask the clock
//    uint8_t AlarmsFired = RTC.checkAlarms();
//    // Then can check if either alarm is fired (there are 2 alarms possible in the chip)
//    // by using a "bitwise and"
//    if(AlarmsFired & 1) {
//      RTC.printTo(Serial); Serial.println(": First alarm has fired!");
//    }
//    if(AlarmsFired & 2) {
//      RTC.printTo(Serial); Serial.println(": Second alarm has fired!");
//    }
  #endif

  loopUserInputs();
  loopModes();
  
  //clock extras
  //showTime();

  FastLED.show();                           //send all the data to the strips
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  //
  //delay(_mainLoopDelay);  //using FastLED.delay instead..
}

