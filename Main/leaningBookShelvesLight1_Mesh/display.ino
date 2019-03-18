/*----------------------------display----------------------------*/

void setupLEDs() {
  
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER_DRAW);

  FastLED.addLeds<WS2812B, _ledDOut0Pin, GRB>(leds[0], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<WS2812B, _ledDOut1Pin, GRB>(leds[1], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<WS2812B, _ledDOut2Pin, GRB>(leds[2], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  //FastLED.addLeds<WS2812B, _ledDOut3Pin, GRB>(leds[3], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  //FastLED.addLeds<WS2812B, _ledDOut4Pin, GRB>(leds[4], _ledNumPerStrip).setCorrection( TypicalSMD5050 );

  _ledGlobalBrightnessCur = _ledGlobalBrightness;
  //FastLED.setBrightness(CRGB(255,255,255));         //set RGB individual brightness
  FastLED.setBrightness(_ledGlobalBrightness);      //set global brightness
  FastLED.setTemperature(UncorrectedTemperature);   //set first temperature

}

