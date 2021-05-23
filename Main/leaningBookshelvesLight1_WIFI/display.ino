/*----------------------------display----------------------------*/
/*----------------------------setup LED----------------------------*/
void setupLED() {
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER_DRAW);

  FastLED.addLeds<WS2812B, _ledDOut0Pin, GRB>(_leds[0], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<WS2812B, _ledDOut1Pin, GRB>(_leds[1], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<WS2812B, _ledDOut2Pin, GRB>(_leds[2], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<WS2812B, _ledDOut3Pin, GRB>(_leds[3], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  // further addition of strips will be seperate ontrols

  _ledGlobalBrightnessCur = _ledGlobalBrightness;
  FastLED.setBrightness(_ledGlobalBrightness);      //set global brightness
  FastLED.setTemperature(UncorrectedTemperature);   //set first temperature
}

void flashLED(byte led) {
  for(int i = 0; i < _ledNumOfStrips; i++) {
    if (led == 0) {
      //
    } else if (led == 1) {
      _leds[i][1] = CRGB::Yellow;
    } else if (led == 2) {
      _leds[i][2] = CRGB::Fuchsia;
    } else if (led == 3) {
      _leds[i][3] = CRGB::Orange;
    } else if (led == 4) {
      _leds[i][4] = CRGB::Violet;
    } else if (led == 5) {
      _leds[i][5] = CRGB::Teal;
    } else if (led == 6) {
      _leds[i][6] = CRGB::Pink;
    } else if (led == 7) {
      _leds[i][7] = CRGB::Green;
    }
  }
  FastLED.show();
  delay(400);
}

/*----------------------------loop led----------------------------*/
void loopLED() {
  FastLED.show();                             // send all the data to the strips
  FastLED.delay(1000 / UPDATES_PER_SECOND);

  if (_modeCur == 8) {
    EVERY_N_MILLISECONDS( 20 ) { _gHue++; }   // slowly cycle the "base color" through the rainbow (for "effect" mode).
  }
}


/*----------------------------led helpers----------------------------*/

void coverageMask() {
  //
}

/*
 Check Segment Endpoints
 usage | checkSegmentEndpoints();
 overlay/place near end of chain
 This places different coloured lights at the ends of the LED strip(s) segments for quick visual feedback of calculations
 */
void checkSegmentEndpoints() {
  for(int i = 0; i < _ledNumOfStrips; i++) {
    fill_solid( _leds[i], _ledNumPerStrip, CRGB(0,0,0));
    
    for(int j = 1; j < _segmentTotal; j++) {
      _leds[i][_ledSegment[j].first] = CRGB(i*20, j*50, 0);
      _leds[i][_ledSegment[j].last] = CRGB(0, j*50, i*20);
    }
  }
}

/*
 Show colour temperature indicator pixel
 usage | showColorTempPx();
 overlay/place near end of chain
 This sets the first LED to show the current colour temperature
 */
void showColorTempPx() {
  _leds[3][_ledSegment[6].first+1] = _colorTempCur; //show indicator pixel
}
