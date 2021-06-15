/*
    'leaningBookshelvesLight1_WIFI' by Thurstan. LED strip bookshelves light.
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
#include <FS.h>                               // a few saved settings
#include <WiFi.h>                             // 
#include <ArduinoJson.h>                      // https://github.com/bblanchon/ArduinoJson
#include <PubSubClient.h>                     // https://github.com/knolleary/pubsubclient
#include <FastLED.h>                          // WS2812B LED strip control and effects
#include "Seeed_MPR121_driver.h"              // Grove - 12 Key Capacitive I2C Touch Sensor V2 (MPR121) - using edited version

/*----------------------------system----------------------------*/
const String _progName = "leaningBookshelvesLight1_WIFI";
const String _progVers = "0.102";             // WIFI init 2 fix

bool USE_SERIAL = true;                       // serial output / turned on/off when flashing board
bool DEBUG_GEN = false;                       // realtime serial debugging output - general
bool DEBUG_OVERLAY = false;                   // show debug overlay on leds (eg. show segment endpoints, center, etc.)
bool DEBUG_MESHSYNC = false;                  // show painless mesh sync by flashing some leds (no = count of active mesh nodes) 
bool DEBUG_COMMS = false;                     // realtime serial debugging output - comms
bool DEBUG_USERINPUT = false;                 // realtime serial debugging output - user input
bool DEBUG_TIME = false;                      // time

bool _firstTimeSetupDone = false;             // starts false //this is mainly to catch an interrupt trigger (which isn't used anymore..) that happens during setup, but is usefull for other things
bool _onOff = false;                          // this should init false, then get activated by input - on/off true/false
bool _shouldSaveSettings = false;             // flag for saving data
bool _runonce = true;                         // flag for sending states when first mesh conection

bool _isBreathing = false;                    // toggle for breath
bool _isBreathOverlaid = false;               // toggle for whether breath is overlaid on top of modes
bool _isBreathingSynced = false;              // breath sync local or global

/*----------------------------pins----------------------------*/
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
bool _sunRiseEnabled = false;
bool _sunSetEnabled = false;
volatile bool _sunRiseTriggered = false;
volatile bool _sunSetTriggered = false;
int _sunRiseStateCur = 0;                     // current sunrise internal state (beginning, rise, end)
int _sunSetStateCur = 0;                      // current sunset internal state (beginning, fall, end)

/*----------------------------buttons----------------------------*/


/*----------------------------touch sensors----------------------------*/
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
bool _fansEnclosureEnabled = false;        // are the enclosure fans on?
//boolean _fansShelvesEnabled[4][8] = { {false,false,false,false,false,false,false,false}, {false,false,false,false,false,false,false,false}, {false,false,false,false,false,false,false,false}, {false,false,false,false,false,false,false,false} };                 // are the fans on?
bool _fansTopEnabled = false;              // is the very top fan on?
byte _fansEnclosureSpeed = 255;               // the speed at which the enclosure fans run.
//byte _fansShelvesSpeed[4][8] = {};            // the speed at which the shelf fans run.
byte _fansTopSpeed = 255;                     // the speed at which the top fan runs.

/*----------------------------LED----------------------------*/
#define MAX_POWER_DRAW 8580                   // limit power draw to ...Amp at 5v - going to be a 12A supply. I think 9.6A normal and 12A max draw + any extra lights, pheripherals, fans, sensors, devices.
#define UPDATES_PER_SECOND 120                // main loop FastLED show delay //100 = FastLED.delay(1000 / UPDATES_PER_SECOND);
const int _ledNumOfStrips = 4;                // 3x LED strips (39, 39, 39, 26)
const int _ledNumPerStrip = 40;               // Xm strip with LEDs ( 1 + 39 )
const int _segmentTotal = 9;                  // total segments (shelves) on each strip ( 1 + 8 )
const int _ledGlobalBrightness = 255;         // global brightness - use this to cap the brightness
int _ledGlobalBrightnessCur = 255;            // current global brightness - adjust this one!
int _ledBrightnessIncDecAmount = 10;          // the brightness amount to increase or decrease

typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;
LED_SEGMENT _ledSegment[_segmentTotal] = { 
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
CRGB _leds[_ledNumOfStrips][_ledNumPerStrip];  // global RGB array matrix
                       
#define TEMPERATURE_0 WarmFluorescent
#define TEMPERATURE_1 StandardFluorescent
#define TEMPERATURE_2 CoolWhiteFluorescent
CHSV startColor( 144, 70, 64 );
CHSV endColor( 31, 71, 69 );
CRGB startColor_RGB( 3, 144, 232 );
CRGB endColor_RGB( 249, 180, 1 );
uint8_t _gHue = 0;                            // rotating "base color"

/*---------------------------- WIFI----------------------------*/
const long _wifiConnectionDelay = 500;

/*----------------------------MQTT----------------------------*/
unsigned long _mqttConnectionPreviousMillis = millis();
const long _mqttConnectionInterval = 60000;

char _mqtt_server[] = MQTT_BROKER_IP;
char _mqtt_port[] = "1883"; //MQTT_BROKER_PORT;
//uint16_t mqtt_broker_port = MQTT_BROKER_PORT;
char _workgroup[] = WORKGROUP_NAME;
char _username[] = MQTT_BROKER_USERNAME;
char _password[] = MQTT_BROKER_PASSWORD;

char _machineId[32] = "";                      // MD5 of chip ID
bool _shouldSaveConfig = false;                // flag for saving data

WiFiClient espClient;
//PubSubClient mqttClient(MQTT_BROKER_IP, MQTT_BROKER_PORT, mqttCallback, espClient);
PubSubClient mqttClient(espClient);

const uint8_t MSG_BUFFER_SIZE = 50;
char m_msg_buffer[MSG_BUFFER_SIZE];           // buffer used to send/receive data with MQTT

// Broadcast states and subscribe to commands
// Topics that require feedback to the server use COMMAND "set" and STATE "status".

//   ....needs an RGB for effects.... !!!

const PROGMEM char* MQTT_LIGHTS_TOPIC_COMMAND = "house/leaningbookshelves1/lights/light/switch"; // receive switch
PROGMEM char* MQTT_LIGHTS_TOPIC_STATE = "house/leaningbookshelves1/lights/light/status";   // send status

const PROGMEM char* MQTT_LIGHTS_BRIGHTNESS_TOPIC_COMMAND = "house/leaningbookshelves1/lights/brightness/set";  // receive set
PROGMEM char* MQTT_LIGHTS_BRIGHTNESS_TOPIC_STATE = "house/leaningbookshelves1/lights/brightness/status"; // send status

const PROGMEM char* MQTT_LIGHTS_MODE_TOPIC_COMMAND = "house/leaningbookshelves1/lights/mode/set";
PROGMEM char* MQTT_LIGHTS_MODE_TOPIC_STATE = "house/leaningbookshelves1/lights/mode/status";

const PROGMEM char* MQTT_LIGHTS_MODE_COLTEMP_TOPIC_COMMAND = "house/leaningbookshelves1/lights/mode/coltemp/set";
PROGMEM char* MQTT_LIGHTS_MODE_COLTEMP_TOPIC_STATE = "house/leaningbookshelves1/lights/mode/coltemp/status";

const PROGMEM char* MQTT_LIGHTS_MODE_EFFECT_TOPIC_COMMAND = "house/leaningbookshelves1/lights/mode/effect/set";
PROGMEM char* MQTT_LIGHTS_MODE_EFFECT_TOPIC_STATE = "house/leaningbookshelves1/lights/mode/effect/status";

const PROGMEM char* MQTT_LIGHTS_MODE_COVERAGE_TOPIC_COMMAND = "house/leaningbookshelves1/lights/mode/coverage/set";
PROGMEM char* MQTT_LIGHTS_MODE_COVERAGE_TOPIC_STATE = "house/leaningbookshelves1/lights/mode/coverage/status";

const PROGMEM char* MQTT_LIGHTS_SUNRISE_TOPIC_COMMAND = "house/leaningbookshelves1/lights/sunrise";
const PROGMEM char* MQTT_SUNRISE_GLOBAL_TOPIC_COMMAND = "house/sunrise";

const PROGMEM char* MQTT_LIGHTS_SUNSET_TOPIC_COMMAND = "house/leaningbookshelves1/lights/sunset";
const PROGMEM char* MQTT_SUNSET_GLOBAL_TOPIC_COMMAND = "house/sunset";

const PROGMEM char* MQTT_LIGHTS_BREATH_TOPIC_COMMAND = "house/leaningbookshelves1/lights/breath";
const PROGMEM char* MQTT_BREATH_GLOBAL_TOPIC_COMMAND = "house/breath";
const PROGMEM char* MQTT_LIGHTS_BREATH_XYZ_TOPIC_COMMAND = "house/leaningbookshelves1/lights/breath/xyz"; 
const PROGMEM char* MQTT_LIGHTS_BREATH_XYZ_MODE_TOPIC_COMMAND = "house/leaningbookshelves1/lights/breath/xyz/mode";

const PROGMEM char* MQTT_DEBUG_GENERAL_TOPIC_COMMAND = "house/leaningbookshelves1/debug/general/set";
PROGMEM char* MQTT_DEBUG_GENERAL_TOPIC_STATE = "house/leaningbookshelves1/debug/general/status";

const PROGMEM char* MQTT_DEBUG_OVERLAY_TOPIC_COMMAND = "house/leaningbookshelves1/debug/overlay/set";
PROGMEM char* MQTT_DEBUG_OVERLAY_TOPIC_STATE = "house/leaningbookshelves1/debug/overlay/status";

const PROGMEM char* MQTT_DEBUG_COMMS_TOPIC_COMMAND = "house/leaningbookshelves1/debug/comms/set";
PROGMEM char* MQTT_DEBUG_COMMS_TOPIC_STATE = "house/leaningbookshelves1/debug/comms/status";

const PROGMEM char* MQTT_DEBUG_RESET_TOPIC_COMMAND = "house/leaningbookshelves1/debug/reset";
const PROGMEM char* MQTT_DEBUG_RESTART_TOPIC_COMMAND = "house/leaningbookshelves1/debug/restart";

const PROGMEM char* MQTT_RESET_GLOBAL_TOPIC_COMMAND = "house/reset";
const PROGMEM char* MQTT_RESTART_GLOBAL_TOPIC_COMMAND = "house/restart";
const PROGMEM char* MQTT_LOCKDOWN_GLOBAL_TOPIC_COMMAND = "house/lockdown";
const PROGMEM char* MQTT_STATUS_REQUEST_TOPIC_COMMAND = "house/leaningbookshelves1/status/request";

//char* _effect = "Normal";
String _modeString = "Fade";                  // Normal


/*----------------------------MAIN----------------------------*/
void setup() {

  if (USE_SERIAL) {
    Serial.begin(115200);
    
    Serial.println();
    Serial.print(_progName);
    Serial.print(" v");
    Serial.print(_progVers);
    Serial.println();
    Serial.print("..");
    Serial.println();
  }
  
  delay(3000);                                // Give the power, LED strip, etc. a couple of secs to stabilise
  
  loadSettings();
  
  setupLED();
    flashLED(1);
  setupUserInputs(); 
    flashLED(2); 
  setupWIFI();
    flashLED(3);
  setupMQTT();
    flashLED(4);
  setupHvac();
    flashLED(5);

  //everything done? ok then..
  if (USE_SERIAL) { Serial.print("Setup done"); }
}


void loop() {

  if(_firstTimeSetupDone == false) {
    if (DEBUG_GEN) { }
    _onOff = true;
    _firstTimeSetupDone = true;               // need this for stuff like setting sunrise, cos it needs the time to have been set
  }

  loopMQTT();
  loopUserInputs();
  loopModes();
  loopHvac();
  loopDebug();
  loopSaveSettings();
  loopLED();
}
