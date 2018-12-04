/*
    'leaningBookshelvesLight1_Mesh' by Thurstan. LED strip bookshelves light.
    Copyright (C) 2018 MTS Standish (mattThurstan)

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

    https://github.com/mattThurstan/
*/


/*----------------------------libraries----------------------------*/
#include <EEPROM.h>                           // a few saved settings
#include <FastLED.h>                          // WS2812B LED strip control and effects
#include <Wire.h>                             // include, but do not need to initialise - for DS3231 & CAP1296
#include <CAP1296.h>                          // Microchip capacitive touch IC - CAP1296 (6 sensors)
#include <PubSubClient.h>                         //https://github.com/knolleary/pubsubclient
#include "painlessMesh.h"
#include <MT_LightControlDefines.h>


/*----------------------------system----------------------------*/
const String _progName = "leaningBookshelvesLight1_Mesh";
const String _progVers = "0.30";              // split to Standalone/WIFI/Mesh
//const int _mainLoopDelay = 0;               // just in case  - using FastLED.delay instead..
#define DEBUG 1                               // 0 or 1
boolean _debugOverlay = false;                // show debug overlay (eg. show segment endpoints)
boolean _firstTimeSetupDone = false;          // starts false //this is mainly to catch an interrupt trigger that happens during setup, but is usefull for other things
volatile boolean _onOff = false;              // this should init false, then get activated by input - on/off true/false

/*----------------------------pins----------------------------*/
//serial TX pin = 0
//serial RX pin = 1
//const int _i2cInterrupt1Pin = 2;              // I2C interrupt pin 1 - DS3231 interrupt pin
//const int _i2cInterrupt2Pin = 3;              // I2C interrupt pin 2
//const int _resetESP8266Pin = 5;               // raise high? to reset
const int _ledDOut0Pin = 14;                  // DOut 0 -> LED strip 0 DIn   - right (short)
const int _ledDOut1Pin = 12;                  // DOut 1 -> LED strip 1 DIn   - middle (long)
const int _ledDOut2Pin = 13;                  // DOut 2 -> LED strip 2 DIn   - left (long)
//const int _ledDOut3Pin = 0;                   // DOut 3 -> LED strip 3 DIn   - SPARE ..for future use
//const int _ledDOut4Pin = 2;                   // DOut 4 -> LED strip 4 DIn   - SPARE
const int _i2cSDApin = 4;                     // SDA (D2)
const int _i2cSCLpin = 5;                     // SCL (D1)

/*----------------------------modes----------------------------*/
const int _modeNum = 9;
const int _modePresetSlotNum = 6;
int _modePreset[_modePresetSlotNum] = { 0, 2, 3, 4, 5, 7 }; //test basic, tap bt to cycle around 6 mode slots   //expand to array or struct later for more presets
volatile int _modeCur = 0;                      // current mode in use - this is not the var you are looking for.. try _modePresetSlotCur
int _modePresetSlotCur = 0;                     // the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput

//#define ARRAYSIZE 10
//String results[ARRAYSIZE] = { "uno", "duo", "tri" };
//char *pointer[] = { "uno", "due",  "due"};
String modeName[_modeNum] = { "Glow", "Sunrise", "Morning", "Day", "Working", "Evening", "Sunset", "Night", "Effect" };
//char *modeName[] = { "Glow", "Sunrise", "Morning", "Day", "Working", "Evening", "Sunset", "Night", "Effect" };


/*-----------------RTC (DS3231 and AT24C32) on I2C------------------*/
boolean _sunRiseEnabled = false;
boolean _sunSetEnabled = false;
int _sunRiseStateCur = 0;                       // current sunrise internal state (beginning, rise, end)
int _sunSetStateCur = 0;                        // current sunset internal state (beginning, fall, end)

/*----------------------------buttons----------------------------*/

/*----------------------------touch sensors----------------------------*/
CAP1296 touch;                                  // CAP1296 on I2C

/*----------------------------LED----------------------------*/
typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;
const int _ledNumOfStrips = 3;                  // 3x LED strips (12, 34, 34)  (not using all 5 yet)
const int _ledNumPerStrip = 35;                 // Xm strip with LEDs
const int _segmentTotal = 10;                   // total segments (shelves) on each strip - not sure ever use this, check  !!!
const int _ledGlobalBrightness = 255;           // global brightness - use this to cap the brightness
int _ledGlobalBrightnessCur = 255;              // current global brightness - adjust this one!
int _ledBrightnessIncDecAmount = 10;            // the brightness amount to increase or decrease
#define UPDATES_PER_SECOND 120                  // main loop FastLED show delay //100
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
                                              
CRGB leds[_ledNumOfStrips][_ledNumPerStrip];    // global RGB array matrix
int _ledState = LOW;                            // use to toggle LOW/HIGH (ledState = !ledState)
#define TEMPERATURE_0 WarmFluorescent
#define TEMPERATURE_1 StandardFluorescent
#define TEMPERATURE_2 CoolWhiteFluorescent
int _colorTempCur = 5;                          // current colour temperature       ???

/*----------------------------Mesh----------------------------*/
char mesh_name[] = MESH_NAME;
char mesh_password[] = MESH_PASSWORD;
uint16_t mesh_port = MESH_PORT;
painlessMesh  mesh;
Scheduler userScheduler; // to control your personal task ??? prob never gonna use

void receivedCallback(uint32_t from, String &msg ) {
  if (DEBUG) { Serial.printf("leaningBookshelvesLight1_Mesh: Received from %u msg=%s\n", from, msg.c_str()); }
  receiveMessage(from, msg);
}

void newConnectionCallback(uint32_t nodeId) {
  if (DEBUG) { Serial.printf("--> leaningBookshelvesLight1_Mesh: New Connection, nodeId = %u\n", nodeId); }
}

void changedConnectionCallback() {
  if (DEBUG) { Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str()); }
}

void nodeTimeAdjustedCallback(int32_t offset) {
  if (DEBUG) { Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset); }
}

void delayReceivedCallback(uint32_t from, int32_t delay) {
  if (DEBUG) { Serial.printf("Delay to node %u is %d us\n", from, delay); }
}

//const PROGMEM char* THESE_LIGHTS_ON = "ON";
//const PROGMEM char* THESE_LIGHTS_OFF = "OFF";
String _modeString = "Glow";


/*----------------------------MAIN----------------------------*/
void setup() {
  
  Serial.begin(115200);
  
  if (DEBUG) {
    Serial.println();
    Serial.print(_progName);
    Serial.print(" v.");
    Serial.print(_progVers);
    Serial.println();
    Serial.print("..");
    Serial.println();
  }
  
  //setupInterrupts();
  
  delay(3000);                                  // give the power, LED strip, etc. a couple of secs to stabilise
  setupLEDs();
  setupUserInputs();                            //
  setupMesh();

  //setSunRise(9, 30);      //TEMP
  
  if (DEBUG) {
  //everything done? ok then..
    Serial.print(F("Setup done"));
    Serial.println("-----");
    Serial.print("Node ID: ");
    Serial.println(mesh.getNodeId());
    Serial.println("-----");
    Serial.println("");
  }
}

void loop() {
  
  if(_firstTimeSetupDone == false) {
    if (DEBUG) {
      
    }
    _firstTimeSetupDone = true;                 // need this for stuff like setting sunrise, cos it needs the time to have been set
  }

  //comms();
  mesh.update();
  loopUserInputs();
  loopModes();
  
  //clock extras
  //showTime();

  if (_debugOverlay) {
    checkSegmentEndpoints();
    //showColorTempPx();
  }

  FastLED.show();                               // send all the data to the strips
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  //
  //delay(_mainLoopDelay);  //using FastLED.delay instead..
}

