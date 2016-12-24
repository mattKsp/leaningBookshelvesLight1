/*----------------------------display----------------------------*/

void setupLEDs() {
  
  //system LEDs
  pinMode(_ledPin, OUTPUT);                 //Setup the internal LED on pin 13
  digitalWrite(_ledPin, _ledState);

  //FastLED
  //FastLED.setMaxPowerInVoltsAndMilliamps(5, 2850);  //limit power draw to 2.85A at 5v (with 3A power supply this gives us a bit of head room for board, lights etc.)
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 5700);  //limit power draw to 5.7A at 5v (with 6A power supply this gives us a bit of head room for board, lights etc.)

  FastLED.addLeds<WS2812B, _ledDOut0Pin, GRB>(leds[0], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<WS2812B, _ledDOut1Pin, GRB>(leds[1], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<WS2812B, _ledDOut2Pin, GRB>(leds[2], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  
  //FastLED.setBrightness(CRGB(255,255,255));         //set RGB individual brightness
  FastLED.setBrightness(_ledGlobalBrightness);      //set global brightness
  FastLED.setTemperature(UncorrectedTemperature);   //set first temperature

}

void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    for(int i = 0; i < _ledNumOfStrips; i++) {
      leds[i][ random16(_ledNumPerStrip) ] += CRGB::White;
    }
  }
}

/*-----------display - status------------*/
void blinkStatusLED() {
  //this delays the whole script. use sparingly
  digitalWrite(_ledPin, HIGH);
  delay(250);
  digitalWrite(_ledPin, LOW);
  delay(250);
  digitalWrite(_ledPin, HIGH);
  delay(250);
  digitalWrite(_ledPin, LOW);
  delay(250);
  digitalWrite(_ledPin, HIGH);
  delay(250);
  digitalWrite(_ledPin, LOW);
}
