/*----------------------------display----------------------------*/

void setupLEDs() {
  
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER_DRAW);

  FastLED.addLeds<WS2812B, _ledDOut0Pin, GRB>(leds[0], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<WS2812B, _ledDOut1Pin, GRB>(leds[1], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<WS2812B, _ledDOut2Pin, GRB>(leds[2], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<WS2812B, _ledDOut3Pin, GRB>(leds[3], _ledNumPerStrip).setCorrection( TypicalSMD5050 );
  //FastLED.addLeds<WS2812B, _ledDOut4Pin, GRB>(leds[4], _ledNumPerStrip).setCorrection( TypicalSMD5050 );

  _ledGlobalBrightnessCur = _ledGlobalBrightness;
  FastLED.setBrightness(_ledGlobalBrightness);      //set global brightness
  FastLED.setTemperature(UncorrectedTemperature);   //set first temperature

}


void coverageMask() {
  if (_coverageCur == 0) { /* "Full" LED coverage - do nothing */ }
  else if (_coverageCur == 1) {
    /* "HiCut" LED coverage - top edge is blacked out */
    for(int i = 0; i < _ledNumOfStrips; i++) {
      //fadeToBlackBy( leds[i], _ledNumPerStrip, 30); 
      if (i == 3) {
        for(int ii3 = ledSegment[6].first; ii3 < (ledSegment[6].first+2); ii3++) {
          leds[i][ii3] = CRGB::Black;
        }
      } else {
        for(int ii = ledSegment[8].first; ii <= ledSegment[8].last; ii++) {
          leds[i][ii] = CRGB::Black;
        }
      }
    } 
  }
  else if (_coverageCur == 2) {
    /* "LowCut" LED coverage - bottom edge is blacked out */
    for(int i = 0; i < _ledNumOfStrips; i++) {
      for(int ii = ledSegment[0].first; ii <= ledSegment[0].last; ii++) {
        leds[i][ii] = CRGB::Black;
      }
    }
  }
  else if (_coverageCur == 3) {
    /* "HiEdgeCut" LED coverage - edges along the top and down the open side are blacked out */
    for(int i = ledSegment[8].first; i <= ledSegment[8].last; i++) {
      leds[0][i] = CRGB::Black;
      leds[1][i] = CRGB::Black;
    }
    for(int ii = ledSegment[6].first; ii <= ledSegment[8].last; ii++) {
      leds[2][ii] = CRGB::Black;
    }
    for(int iii = ledSegment[6].first; iii < (ledSegment[6].first+2); iii++) {
      leds[3][iii] = CRGB::Black;
    }
  /*  for(int iiii = ledSegment[1].first; iiii <= ledSegment[1].last; iiii++) {
      leds[0][iiii] = CRGB::Black;
      leds[1][iiii] = CRGB::Black;
      leds[2][iiii] = CRGB::Black;
    }
  */  
  }
  else if (_coverageCur == 4) {
    /* "FullEdgeCut" LED coverage - edges along the top, down the open side and along the bottom are blacked out */
    for(int i = ledSegment[8].first; i <= ledSegment[8].last; i++) {
      leds[0][i] = CRGB::Black;
      leds[1][i] = CRGB::Black;
    }
    for(int ii = ledSegment[6].first; ii <= ledSegment[8].last; ii++) {
      leds[2][ii] = CRGB::Black;
    }
    for(int iii = ledSegment[0].first; iii < (ledSegment[6].first+2); iii++) {
      leds[3][iii] = CRGB::Black;
    }
    for(int iiii = ledSegment[0].first; iiii <= ledSegment[0].last; iiii++) {
      leds[0][iiii] = CRGB::Black;
      leds[1][iiii] = CRGB::Black;
      leds[2][iiii] = CRGB::Black;
    }
  }
  else if (_coverageCur == 5) {
    /* "BackProfile" LED coverage - edges following the back profile (curves) are blacked out */
    for(int i = ledSegment[8].first; i <= ledSegment[8].last; i++) {
      leds[0][i] = CRGB::Black;
      leds[1][i] = CRGB::Black;
    }
    for(int ii = ledSegment[7].first; ii <= ledSegment[7].last; ii++) {
      leds[2][ii] = CRGB::Black;
    }
    for(int iii = ledSegment[5].first; iii < (ledSegment[6].first+2); iii++) {
      leds[3][iii] = CRGB::Black;
    }
  }
}
