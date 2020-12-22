/*
    'leaningBookshelvesLight1_Mesh' by Thurstan. LED strip bookshelves light.
    Copyright (C) 2020 MTS Standish (Thurstan|mattKsp)
    
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
const String _progVers = "0.520";             // new LED setup

boolean DEBUG_GEN = false;                    // realtime serial debugging output - general
boolean DEBUG_OVERLAY = false;                // show debug overlay on leds (eg. show segment endpoints, center, etc.)
boolean DEBUG_MESHSYNC = false;               // show painless mesh sync by flashing some leds (no = count of active mesh nodes) 
boolean DEBUG_COMMS = false;                  // realtime serial debugging output - comms
boolean DEBUG_USERINPUT = false;              // realtime serial debugging output - user input
boolean DEBUG_TIME = false;                   // time

boolean _firstTimeSetupDone = false;          // starts false //this is mainly to catch an interrupt trigger (which isn't used anymore..) that happens during setup, but is usefull for other things
volatile boolean _onOff = false;              // this should init false, then get activated by input - on/off true/false
bool shouldSaveSettings = false;              // flag for saving data
bool runonce = true;                          // flag for sending states when first mesh conection
//const int _mainLoopDelay = 0;               // just in case  - using FastLED.delay instead..

bool _isBreathing = false;                    // toggle for breath
bool _isBreathOverlaid = false;               // toggle for whether breath is overlaid on top of modes
bool _isBreathingSynced = false;              // breath sync local or global

/*----------------------------pins----------------------------*/

/*
 / Pinout Wemos D32 Pro (ESP32-WROVER)
 / ..remember to disconnect all pins before re-flashing

 / 36 - VP (Analog Input) (RTC_GPIO0 interrupt 0)
 / 39 - VN (Analog Input) (RTC_GPIO0 interrupt 3)
 / 34 - ADC1_CH6 (Analog Input)
 / 32 - ADC1_CH4 / TFT_LED (Analog Input)
//33 - ADC1_CH5 / TFT_RST (Analog Input)
//25 - DAC_1 (Analog Output)
//26 - DAC_2 (Analog Output)
//27 - TFT_DC
//14 - TFT_CS
//12 - TS_CS
//13

//23 - MOSI
//22 - SCL
//1 - TX
//3 - RX
//21 - SDA
//19 - MISO
//18 - SCK
//5 - SS
//(17) NC
//(16) NC
//4
//0
//2
//15

// 5v regulated power is connected to USB (VBUS)
*/

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
int _modePreset[_modePresetSlotNum] = { 0, 2, 3, 4, 5, 7 }; //test basic, tap bt to cycle around 6 mode slots   //expand to array or struct later for more presets
volatile int _modeCur = 0;                    // current mode in use - this is not the var you are looking for.. try _modePresetSlotCur
int _modePresetSlotCur = 0;                   // the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput
String modeName[_modeNum] = { "Glow", "Sunrise", "Morning", "Day", "Working", "Evening", "Sunset", "Night", "Effect" };
int _saveCurMode = 0;

const int _colorTempNum = 3;                  // 3 for now
int _colorTempCur = 1;                        // current colour temperature
String colorTempName[_colorTempNum] = { "Warm", "Standard", "CoolWhite" }; // color temperature sub-mode names for the main "Working" mode.

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

/*----------------------------LED----------------------------*/
// might limit power draw even further if add usb charge ports to the system
// or use usb chips and change power draw if usb device attached and charging
#define MAX_POWER_DRAW 5700                   // limit power draw to ...Amp at 5v
typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;
const int _ledNumOfStrips = 4;                // 3x LED strips (39, 39, 39, 24)
const int _ledNumPerStrip = 40;               // Xm strip with LEDs (1 + 39)
const int _segmentTotal = 9;                 // total segments (shelves) on each strip (1 + 8)
const int _ledGlobalBrightness = 255;         // global brightness - use this to cap the brightness
int _ledGlobalBrightnessCur = 255;            // current global brightness - adjust this one!
int _ledBrightnessIncDecAmount = 10;          // the brightness amount to increase or decrease
#define UPDATES_PER_SECOND 120                // main loop FastLED show delay //100
//need to add 1 led to the beginning to use as a blank to jump from 3.3 to 5v (cheap hack for level shifting)

LED_SEGMENT ledSegment[_segmentTotal] = { 
  { 0, 0, 1 }, //blank for level shifting hack and debug status
  { 1, 2, 2 }, 
  { 3, 8, 6 }, 
  { 9, 14, 6 },
  { 15, 19, 5 },
  { 20, 24, 5 },
  { 25, 29, 5 },
  { 30, 33, 4 },
  { 34, 39, 6 }
};
CHSV startColor( 144, 70, 64 );
CHSV endColor( 31, 71, 69 );
CRGB startColor_RGB( 3, 144, 232 );
CRGB endColor_RGB( 249, 180, 1 );
                                              
CRGB leds[_ledNumOfStrips][_ledNumPerStrip];  // global RGB array matrix
int _ledState = LOW;                          // use to toggle LOW/HIGH (ledState = !ledState)

#define TEMPERATURE_0 WarmFluorescent
#define TEMPERATURE_1 StandardFluorescent
#define TEMPERATURE_2 CoolWhiteFluorescent

/*----------------------------Mesh----------------------------*/
painlessMesh  mesh;
//char mesh_name[] = MESH_NAME;
//char mesh_password[] = MESH_PASSWORD;
//uint16_t mesh_port = MESH_PORT;
String _modeString = "Glow";
uint32_t id = DEVICE_ID_BRIDGE1;

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
  
  Serial.begin(115200);
  
  Serial.println();
  Serial.print(_progName);
  Serial.print(" v");
  Serial.print(_progVers);
  Serial.println();
  Serial.print("..");
  Serial.println();

  delay(3000);                                // Give the power, LED strip, etc. a couple of secs to stabilise
  setupLEDs();
  setupUserInputs();                          
  setupMesh();

  //everything done? ok then..
  Serial.print(F("Setup done"));
  Serial.println("-----");
  Serial.print(F("Device Node ID is "));
  String s = String(mesh.getNodeId());
  Serial.println(s);
  Serial.println("-----");
  Serial.println("");

  delay(1);
}

void loop() {
  
  if(_firstTimeSetupDone == false) {
    if (DEBUG_GEN) { }
    _firstTimeSetupDone = true;               // need this for stuff like setting sunrise, cos it needs the time to have been set
  }

  mesh.update();
  loopUserInputs();
  loopModes();
  
  if (DEBUG_OVERLAY) {
    checkSegmentEndpoints();
    //showColorTempPx();
  }
  
  EVERY_N_SECONDS(30) {                       // too much ???
    if (shouldSaveSettings == true)
    { 
      //saveSettings(); 
      shouldSaveSettings = false; 
    }
  }

  FastLED.show();                             // send all the data to the strips
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  //
  //delay(_mainLoopDelay);  //using FastLED.delay instead..
}
