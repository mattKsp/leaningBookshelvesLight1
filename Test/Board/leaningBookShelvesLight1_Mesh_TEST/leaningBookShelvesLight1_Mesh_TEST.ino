
/*----------------------------libraries----------------------------*/
#include <FastLED.h>                          // WS2812B LED strip control and effects

/*----------------------------pins----------------------------*/
// 5v regulated power is connected to USB (VBUS).
// I2C (for touch) is connected through a dedicated connector.
// LED data lines are level shifted using an SN74HC14N.
const int _ledDOut0Pin = 4;                   // DOut 0 -> LED strip 0 DIn   - right 2 (short)
const int _ledDOut1Pin = 0;                   // DOut 1 -> LED strip 1 DIn   - right (long)
const int _ledDOut2Pin = 2;                   // DOut 2 -> LED strip 2 DIn   - middle
const int _ledDOut3Pin = 15;                  // DOut 3 -> LED strip 3 DIn   - left

/*----------------------------LED----------------------------*/
// might limit power draw even further if add usb charge ports to the system
// or use usb chips and change power draw if usb device attached and charging
// 39 x 0.02A = 0.78A x 3 = 2.34 x 3 = 7.02
// 26 x 0.02A = 0.52A x 3 = 1.56
// 7.02 + 1.56 = 8.58 Amp draw for all lights on full white.
// control board 500mA
// esp8266 device 250mA
#define MAX_POWER_DRAW 8580                   // limit power draw to ...Amp at 5v - going to be a 12A supply. I think 9.6A normal and 12A max draw + any extra lights, pheripherals, fans, sensors, devices.
typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;
const int _ledNumOfStrips = 4;                // 3x LED strips (39, 39, 39, 26)
const int _ledNumPerStrip = 39;               // Xm strip with LEDs
const int _segmentTotal = 8;                  // total segments (shelves) on each strip
const int _ledGlobalBrightness = 255;         // global brightness - use this to cap the brightness
#define UPDATES_PER_SECOND 120                // main loop FastLED show delay //100 = FastLED.delay(1000 / UPDATES_PER_SECOND);
LED_SEGMENT ledSegment[_segmentTotal] = { 
  { 0, 1, 2 }, 
  { 2, 7, 6 },    // shelves start
  { 8, 13, 6 },
  { 14, 18, 5 },
  { 19, 23, 5 },
  { 24, 28, 5 },  // short end upright ends after 2nd pixel at this level
  { 29, 32, 4 },
  { 33, 38, 6 }
};
                                         
//CRGB leds[_ledNumOfStrips][_ledNumPerStrip];  // global RGB array matrix
CRGB leds[_ledNumPerStrip];

boolean flip = true;

void setup() {
  Serial.begin(115200);
  Serial.println("leaningBookShelvesLight1_Mesh_TEST");
  setupLEDs();
}

void loop() {

  if (flip) {
    //for(int i = 0; i < _ledNumOfStrips; i++) {
      //fadeToBlackBy( leds[i], _ledNumPerStrip, 30); 
      fadeToBlackBy( leds, _ledNumPerStrip, 30); 
    //}
  } else {
    //for(int i = 0; i < _ledNumOfStrips; i++) {
      //fill_gradient_RGB(leds[i], ledSegment[0].first, CRGB(32, 32, 32), ledSegment[_segmentTotal-1].last, CRGB::Black );
      fill_gradient_RGB(leds, ledSegment[0].first, CRGB(32, 32, 32), ledSegment[_segmentTotal-1].last, CRGB::Black );
    //}
  }
  
  FastLED.show();                             // send all the data to the strips
  FastLED.delay(1000 / UPDATES_PER_SECOND);

  EVERY_N_MILLISECONDS( 300 ) { flip == !flip; }
}
