/*
    'leaningBookshelvesLight1_Mesh' by Thurstan. LED strip bookshelves light.
    Copyright (C) 2021 MTS Standish (Thurstan|mattKsp)
    
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

// Wemos D32 Pro (ESP32-WROVER)

/*----------------------------libraries----------------------------*/
#include <MT_LightControlDefines.h>
//#include <FS.h>                               // a few saved settings
#include <FastLED.h>                          // WS2812B LED strip control and effects
#include "Seeed_MPR121_driver.h"              // Grove - 12 Key Capacitive I2C Touch Sensor V2 (MPR121) - using edited version
#include "painlessMesh.h"                     // https://github.com/gmag11/painlessMesh

/*----------------------------system----------------------------*/
const String _progName = "leaningBookshelvesLight1_Mesh";
const String _progVers = "0.528";             // turn off Serial to track down flickering

boolean DEBUG_GEN = false;                    // realtime serial debugging output - general
boolean DEBUG_OVERLAY = false;                // show debug overlay on leds (eg. show segment endpoints, center, etc.)
boolean DEBUG_MESHSYNC = false;               // show painless mesh sync by flashing some leds (no = count of active mesh nodes) 
boolean DEBUG_COMMS = false;                  // realtime serial debugging output - comms
boolean DEBUG_USERINPUT = false;              // realtime serial debugging output - user input
boolean DEBUG_TIME = false;                   // time

boolean _firstTimeSetupDone = false;          // starts false //this is mainly to catch an interrupt trigger (which isn't used anymore..) that happens during setup, but is usefull for other things
bool _onOff = false;                          // this should init false, then get activated by input - on/off true/false
bool _shouldSaveSettings = false;             // flag for saving data
bool runonce = true;                          // flag for sending states when first mesh conection
//const int _mainLoopDelay = 0;               // just in case  - using FastLED.delay instead..

bool _isBreathing = false;                    // toggle for breath
bool _isBreathOverlaid = false;               // toggle for whether breath is overlaid on top of modes
bool _isBreathingSynced = false;              // breath sync local or global

/*----------------------------pins----------------------------*/
// 5v regulated power is connected to USB (VBUS).
// I2C (for touch) is connected through a dedicated connector.
// [ LED data lines are level shifted using an SN74HC14N - this has issues - so reverted to level shift hack ]
const int _ledDOut0Pin = 4;                   // DOut 0 -> LED strip 0 DIn   - right 2 (short)
const int _ledDOut1Pin = 0;                   // DOut 1 -> LED strip 1 DIn   - right (long)
const int _ledDOut2Pin = 2;                   // DOut 2 -> LED strip 2 DIn   - middle
const int _ledDOut3Pin = 15;                  // DOut 3 -> LED strip 3 DIn   - left
//const int _ledDOut4Pin = 27;                  // DOut 4 -> LED strip 4 DIn   - SPARE
//const int _ledDOut4Pin = 14;                  // DOut 4 -> LED strip 4 DIn   - SPARE
//const int _ledDOut4Pin = 12;                  // DOut 4 -> LED strip 4 DIn   - SPARE
//const int _ledDOut4Pin = 13;                  // DOut 4 -> LED strip 4 DIn   - SPARE
 
/*----------------------------modes----------------------------*/
const int _modeNum = 9;
const int _modePresetSlotNum = 6;
String _modeName[_modeNum] = { "Glow", "Sunrise", "Morning", "Day", "Working", "Evening", "Sunset", "Night", "Effect" };
volatile int _modeCur = 0;                    // current mode in use - this is not the var you are looking for.. try _modePresetSlotCur
int _modePreset[_modePresetSlotNum] = { 0, 2, 3, 4, 5, 7 }; //test basic, tap bt to cycle around 6 mode slots   //expand to array or struct later for more presets
int _modePresetSlotCur = 0;                   // the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput
int _saveCurMode = 0;                         // temp

const int _colorTempNum = 3;                  // 3 for now
int _colorTempCur = 1;                        // current colour temperature
String _colorTempName[_colorTempNum] = { "Warm", "Standard", "CoolWhite" }; // color temperature sub-mode names for the main "Working" mode.

const int _effectNum = 6;
int _effectCur = 0;
String _effectName[_effectNum] = { "Fire2012", "Confetti", "AddGlitter", "Rainbow", "RainbowWithGlitter", "Rain" };

const int _coverageNum = 6;                   // 6 for now. LED coverage mask for whole structure.
int _coverageCur = 0;                         // current coverage layer in use.
String _coverageName[_coverageNum] = {"Full", "HiCut", "LowCut", "HiEdgeCut", "FullEdgeCut", "BackProfile" };

/*-----------------sunrise/set------------------*/
boolean _sunRiseEnabled = false;
boolean _sunSetEnabled = false;
volatile boolean _sunRiseTriggered = false;
volatile boolean _sunSetTriggered = false;
int _sunRiseStateCur = 0;                     // current sunrise internal state (beginning, rise, end)
int _sunSetStateCur = 0;                      // current sunset internal state (beginning, fall, end)

/*----------------------------buttons----------------------------*/

/*----------------------------touch sensors----------------------------*/
// MPR121 connected via I2C
Mpr121 mpr121;                                // init MPR121 on I2C
u16 touch_status_flag[CHANNEL_NUM] = { 0 };   // u16 = unsigned short

/*----------------------------HVAC - Inc. Control enclosure and Bookshelves----------------------------*/
/*-- temperature sensor for the enclosure and shelving --*/
byte _temperatureEnclosureCur = 1;            // the current temperature in the enclosure
//byte _temperatureShelvesCur[4][8] = { {1,1,1,1,1,1,0,0}, {1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1}, {1,1,1,1,1,1,1,1} };
byte _temperatureTopCur = 1;                  // the current temperature on the longest leg at the very top of the bookshelves.
byte _temperatureEnclosureOn[2] = { 28, 38 };  // temp at which the fans are turned on (temp rising) and temp at which the fans are fully On.
//const byte _temperatureEnclosureOnMax = 38;   // temp at which the fans are fully On.
byte _temperatureEnclosureOff[2] = { 24, 20 };   // temp at which the fans start slowing down (turning off, temp falling) and temp at which the fans are fully Off.
//const byte _temperatureEnclosureOffMin = 20;  // temp at which the fans are fully Off.
/*-- fans for the enclosure and shelving --*/
boolean _fansEnclosureEnabled = false;        // are the enclosure fans on?
//boolean _fansShelvesEnabled[4][8] = { {false,false,false,false,false,false,false,false}, {false,false,false,false,false,false,false,false}, {false,false,false,false,false,false,false,false}, {false,false,false,false,false,false,false,false} };                 // are the fans on?
boolean _fansTopEnabled = false;              // is the very top fan on?
byte _fansEnclosureSpeed = 255;               // the speed at which the enclosure fans run.
//byte _fansShelvesSpeed[4][8] = {};            // the speed at which the shelf fans run.
byte _fansTopSpeed = 255;                     // the speed at which the top fan runs.

/*----------------------------LED----------------------------*/
// might limit power draw even further if add usb charge ports to the system
// or use usb chips and change power draw if usb device attached and charging
// 39 x 0.02A = 0.78A x 3 = 2.34 x 3 = 7.02
// 26 x 0.02A = 0.52A x 3 = 1.56
// 7.02 + 1.56 = 8.58 Amp draw for all current lights on full white. Adjust when adding extra strips
// control board 500mA
// esp8266 device 250mA
#define MAX_POWER_DRAW 8580                   // limit power draw to ...Amp at 5v - going to be a 12A supply. I think 9.6A normal and 12A max draw + any extra lights, pheripherals, fans, sensors, devices.
typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;
const int _ledNumOfStrips = 4;                // 3x LED strips (39, 39, 39, 26)
const int _ledNumPerStrip = 40;               // Xm strip with LEDs ( 1 + 39 )
const int _segmentTotal = 9;                  // total segments (shelves) on each strip ( 1 + 8 )
const int _ledGlobalBrightness = 255;         // global brightness - use this to cap the brightness
int _ledGlobalBrightnessCur = 255;            // current global brightness - adjust this one!
int _ledBrightnessIncDecAmount = 10;          // the brightness amount to increase or decrease
#define UPDATES_PER_SECOND 120                // main loop FastLED show delay //100 = FastLED.delay(1000 / UPDATES_PER_SECOND);
LED_SEGMENT ledSegment[_segmentTotal] = { 
  { 0, 0, 1 },    // blank for level shifting hack and debug status
  { 1, 2, 2 }, 
  { 3, 8, 6 },    // shelves start
  { 9, 14, 6 },
  { 15, 19, 5 },
  { 20, 24, 5 },
  { 25, 29, 5 },  // short end upright ends after 2nd pixel at this level
  { 30, 33, 4 },
  { 34, 39, 6 }
};
//LED_SEGMENT ledSegment[_segmentTotal] = { 
//  { 0, 1, 2 }, 
//  { 2, 7, 6 },    // shelves start
//  { 8, 13, 6 },
//  { 14, 18, 5 },
//  { 19, 23, 5 },
//  { 24, 28, 5 },  // short end upright ends after 2nd pixel at this level
//  { 29, 32, 4 },
//  { 33, 38, 6 }
//};
CHSV startColor( 144, 70, 64 );
CHSV endColor( 31, 71, 69 );
CRGB startColor_RGB( 3, 144, 232 );
CRGB endColor_RGB( 249, 180, 1 );
uint8_t _gHue = 0; // rotating "base color"
                                              
CRGB leds[_ledNumOfStrips][_ledNumPerStrip];  // global RGB array matrix

#define TEMPERATURE_0 WarmFluorescent
#define TEMPERATURE_1 StandardFluorescent
#define TEMPERATURE_2 CoolWhiteFluorescent

/*----------------------------Mesh----------------------------*/
painlessMesh  mesh;
//char mesh_name[] = MESH_NAME;
//char mesh_password[] = MESH_PASSWORD;
//uint16_t mesh_port = MESH_PORT;
String _modeString = "Glow";
uint32_t id_bridge1 = DEVICE_ID_BRIDGE1;

void receivedCallback(uint32_t from, String &msg ) {
  if (DEBUG_COMMS) { Serial.printf("leaningBookshelvesLight1_Mesh: Received from %u msg=%s\n", from, msg.c_str()); }
  receiveMessage(from, msg);
}

void newConnectionCallback(uint32_t nodeId) {
  if (runonce == true) {
    publishStatusAll(false);
    runonce = false;
  }
  if (DEBUG_COMMS) { Serial.printf("--> leaningBookshelvesLight1_Mesh: New Connection, nodeId = %u\n", nodeId); }
}

void changedConnectionCallback() {
  if (DEBUG_COMMS) { Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str()); }
}

void nodeTimeAdjustedCallback(int32_t offset) {
  if (DEBUG_COMMS) { Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset); }
}

void delayReceivedCallback(uint32_t from, int32_t delay) {
  if (DEBUG_COMMS) { Serial.printf("Delay to node %u is %d us\n", from, delay); }
}


/*----------------------------MAIN----------------------------*/
void setup() {
/*  
  Serial.begin(115200);
  
  Serial.println();
  Serial.print(_progName);
  Serial.print(" v");
  Serial.print(_progVers);
  Serial.println();
  Serial.print("..");
  Serial.println();
*/
  delay(3000);                                // Give the power, LED strip, etc. a couple of secs to stabilise
  setupLEDs();
  setupUserInputs();                          
  setupMesh();
  setupHvac();
/*
  //everything done? ok then..
  Serial.print(F("Setup done"));
  Serial.println("-----");
  Serial.print(F("Device Node ID is "));
  String s = String(mesh.getNodeId());
  Serial.println(s);
  Serial.println("-----");
  Serial.println("");
*/
  delay(1);
}

void loop() {
  
  if(_firstTimeSetupDone == false) {
    if (DEBUG_GEN) { }
    _onOff = true;
    _firstTimeSetupDone = true;               // need this for stuff like setting sunrise, cos it needs the time to have been set
  }

  mesh.update();
  loopUserInputs();
  loopModes();
  //loopHvac();
  
  if (DEBUG_OVERLAY) {
    checkSegmentEndpoints();
    showColorTempPx();
  }
  
  EVERY_N_SECONDS(30) {                       // too much ???
    if (_shouldSaveSettings == true)
    { 
      //saveSettings(); 
      _shouldSaveSettings = false; 
    }
  }

  FastLED.show();                             // send all the data to the strips
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  //
  //delay(_mainLoopDelay);  //using FastLED.delay instead..

  if (_modeCur == 8) {
    EVERY_N_MILLISECONDS( 20 ) { _gHue++; } // slowly cycle the "base color" through the rainbow (for "effect" mode).
  }
}
