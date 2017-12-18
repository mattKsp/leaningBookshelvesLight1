/*
    'leaningBookshelvesLight1_B' by Thurstan. LED strip bookshelves light.
    Copyright (C) 2017  MTS Standish (Thurstan|mattKsp)

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

/*
 * Moved to a custom PCB and added WIFI for a mesh network.
 * Board - Arduino Pro Mini (5v, ATmega328, 16mhz clock speed, 32KB Flash, 2KB SRAM, 1KB EEPROM)
 * WIFI - ESP8266 (communication via serial)
 * 
 * ...****, mucked up the board.. TX is not corrected, RX is WRONGLY connected to ESP TX. will have fix it in post.
*/


#define DEBUG 1                               //comment/un-comment
boolean _debugOverlay = false;                //show debug overlay (eg. show segment endpoints)
#ifdef DEBUG     
//#define
#endif

/*----------------------------libraries----------------------------*/
#include <SoftwareSerial.h>
#include <EEPROM.h>                           //a few saved settings
#include <DS3231_Simple.h>                    //DS3231 realtime clock (with AT24C32 memory backback)
#include <FastLED.h>                          //WS2812B LED strip control and effects
#include <MT_BlinkStatusLED.h>
#include <Wire.h>                             //include, but do not need to initialise - for DS3231

/*----------------------------arduino pro mini pins----------------------------*/
//serial TX pin = 0
//serial RX pin = 1
const int _i2cInterrupt1Pin = 2;              //I2C interrupt pin 1 - DS3231 interrupt pin
const int _i2cInterrupt2Pin = 3;              //I2C interrupt pin 2
const int _resetESP8266Pin = 5;               //raise high? to reset
const int _ledDOut0Pin = 6;                   //DOut 0 -> LED strip 0 DIn   - right (short)
const int _ledDOut1Pin = 7;                   //DOut 1 -> LED strip 1 DIn   - middle (long)
const int _ledDOut2Pin = 8;                   //DOut 2 -> LED strip 2 DIn   - left (long)
const int _ledDOut3Pin = 9;                   //DOut 3 -> LED strip 3 DIn   - SPARE ..for future use
const int _ledDOut4Pin = 10;                  //DOut 4 -> LED strip 4 DIn   - SPARE
SoftwareSerial softSerial(11, 12);              //RX, TX
MT_BlinkStatusLED statusLED(13);              //setup status LED (internal) on pin 13
const int _i2cSDApin = 18;                    //SDA (A4=18)
const int _i2cSCLpin = 19;                    //SDA (A5=19)

/*----------------------------system----------------------------*/
const String _progName = "bookShelvesLight1_B";
const String _progVers = "0.11";              //fork from bookShelvesLight1_A - moved over to custom board
//const int _mainLoopDelay = 0;               //just in case  - using FastLED.delay instead..
boolean _firstTimeSetupDone = false;          //starts false //this is mainly to catch an interrupt trigger that happens during setup, but is usefull for other things
volatile boolean _onOff = false;              //this should init false, then get activated by input - on/off true/false

/*----------------------------serial communications----------------------------*/
//http://forum.arduino.cc/index.php?topic=396450
const byte _numSerialChars = 32;
char _receivedSerialChars[_numSerialChars];
char _tempSerialChars[_numSerialChars];                     //temporary array for use when parsing
char _messageFromSerial[_numSerialChars] = {0};           //variables to hold the parsed data
int _integer1FromSerial = 0;
int _integer2FromSerial = 0;
boolean _newSerialData = false;
//String _inputString = "";                     // a string to hold incoming data     //OLD
//boolean _stringComplete = false;              // whether the string is complete    //OLD

/*----------------------------modes----------------------------*/
const int _modeNum = 9;
const int _modePresetSlotNum = 6;
int _modePreset[_modePresetSlotNum] = { 0, 2, 3, 4, 5, 7 }; //test basic, tap bt to cycle around 6 mode slots   //expand to array or struct later for more presets
volatile int _modeCur = 0;                    //current mode in use - this is not the var you are looking for.. try _modePresetSlotCur
int _modePresetSlotCur = 0;                   //the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput

/*-----------------RTC (DS3231 and AT24C32) on I2C------------------*/
DS3231_Simple RTC;                            //init realtime clock
#define DS3231_I2C_ADDRESS 0x68               //default - only used for interrupt kick
boolean _sunRiseEnabled = false;
//volatile boolean _sunRiseTriggered = false;
boolean _sunSetEnabled = false;
//volatile boolean _sunSetTriggered = false;
//volatile boolean _sunRiseSetTriggered = false;
int _sunRiseStateCur = 0;                     //current sunrise internal state (beginning, rise, end)
int _sunSetStateCur = 0;                      //current sunset internal state (beginning, fall, end)

/*----------------------------buttons----------------------------*/

/*----------------------------touch sensors----------------------------*/

/*----------------------------LED----------------------------*/
typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;
const int _ledNumOfStrips = 3;                //3x LED strips (12, 34, 34)  (not using all 5 yet)
const int _ledNumPerStrip = 35;               //Xm strip with LEDs
const int _segmentTotal = 10;                 //total segments (shelves) on each strip - not sure ever use this, check  !!!
const int _ledGlobalBrightness = 255;         //global brightness - use this to cap the brightness
int _ledGlobalBrightnessCur = 255;            //current global brightness - adjust this one!
int _ledBrightnessIncDecAmount = 10;          //the brightness amount to increase or decrease
#define UPDATES_PER_SECOND 120                //main loop FastLED show delay //100
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
                                              
CRGB leds[_ledNumOfStrips][_ledNumPerStrip];  //global RGB array matrix
int _ledState = LOW;                          //use to toggle LOW/HIGH (ledState = !ledState)
#define TEMPERATURE_0 WarmFluorescent
#define TEMPERATURE_1 StandardFluorescent
#define TEMPERATURE_2 CoolWhiteFluorescent
int _colorTempCur = 5;                        //current colour temperature       ???

/*----------------------------MAIN----------------------------*/
void setup() {
  
  Serial.begin(9600);
  //_inputString.reserve(200);                 //reserve 200 bytes for the inputString:    ???
  
  #ifdef DEBUG
    softSerial.begin(4800);
    softSerial.println();
    softSerial.print(_progName);
    softSerial.print(" v.");
    softSerial.print(_progVers);
    softSerial.println();
    softSerial.print("..");
    softSerial.println();
  #endif
  
  RTC.begin();                                //DS3231 RealTimeClock begin..
  //RTC.formatEEPROM();
  setupInterrupts();
  
  delay(3000);                                //give the power, LED strip, etc. a couple of secs to stabilise
  setupLEDs();
  setupUserInputs();                          //

  //RTC.promptForTimeAndDate(Serial);
  //Serial.println(F("Current date/time = "));
  //RTC.printTo(Serial);
  
  //setSunRise(9, 30);      //TEMP
  DS3231kickInterrupt();  //TEMP util
  
  #ifdef DEBUG
  //everything done? ok then..
    softSerial.print(F("Setup done"));
    softSerial.println();
  #endif
  statusLED.Blink3();
}

void loop() {
  
  if(_firstTimeSetupDone == false) {
    #ifdef DEBUG
    //
    #endif
    _firstTimeSetupDone = true;               //need this for stuff like setting sunrise, cos it needs the time to have been set
  }

  comms();
//  if (_stringComplete) {
//    Serial.println(_inputString);
//    _inputString = "";                      //clear the string:
//    _stringComplete = false;
//  }
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

  loopUserInputs();
  loopModes();
  
  //clock extras
  //showTime();

  if (_debugOverlay) {
    checkSegmentEndpoints();
    //showColorTempPx();
  }

  FastLED.show();                           //send all the data to the strips
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  //
  //delay(_mainLoopDelay);  //using FastLED.delay instead..
}




