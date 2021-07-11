/*----------------------------mode----------------------------*/

/*
 * 0 - glow     (static/moving)
 * 1 - sunrise  (static/moving)
 * 2 - morning  (static/moving)
 * 3 - day      (static/moving) (full bright/overcast/clearbluesky)
 * 4 - working  (static) (full bright) (colour temperature sub-mode)
 * 5 - evening  (static/moving)
 * 6 - sunset   (static/moving)
 * 7 - night    (static/moving)
 * 8 - effect   (moving) (could use static to pause it?)
 * 
 * gradient fade (top/mid/bot) (all can invert) (overlay/fx?)
 * moveable/resizable bright bar (overlay/fx?) (might use brightness +/- intputs to move up/down)
 */

/*
  select mode to run based upon mode var, set by input
  called from main loop
  (select from preset array) ..later
  numbers denote slots
  slots are filled (by hand for now) with named functions
  ..needs an override aswell
  modes are represented as numbers, but have names if needed
  modes are selected individualy or in a present sequence

 */
void loopModes() {
  if (_onOff) {
    if(_modeCur == 0) {       mode0(); } 
    else if(_modeCur == 1) {  mode1(); }
    else if(_modeCur == 2) {  mode2(); }
    else if(_modeCur == 3) {  mode3(); }
    else if(_modeCur == 4) {  mode4(); }
    else if(_modeCur == 5) {  mode5(); }
    else if(_modeCur == 6) {  mode6(); }
    else if(_modeCur == 7) {  mode7(); }
    else if(_modeCur == 8) {  mode8(); }
  }
  else { //turn off - fade everything to black
    for(int i = 0; i < _ledNumOfStrips; i++) {
      fadeToBlackBy( _leds[i], _ledNumPerStrip, 30); 
    } 
  } //END onOff else
  
//  coverageMask();   // see display tab
}

/*----------------------------modes----------------------------*/
void mode0() {
  //name should be glow!
  //bot fade up
  for(int i = 0; i < _ledNumOfStrips; i++) {
    //fill_gradient_RGB(_leds[i], _ledSegment[1].first, CRGB(32, 32, 32), _ledSegment[_segmentTotal-1].last, CRGB::Black );
    fadeToBlackBy( _leds[i], _ledNumPerStrip, 30); 
    fill_gradient_RGB(_leds[i], _ledSegment[1].first, CRGB(32, 32, 32), _topLed, CRGB::Black );
    //_leds[i][0] = CRGB::Black;                                     // hack
  }
}

//mode1 - see 'sunRiseSet'

void mode2() {
  //name is morning!
  for(int i = 0; i < _ledNumOfStrips; i++) {
    //fill_solid( leds[i], _ledNumPerStrip, CRGB(0, 0, 255));
    //fill_gradient_RGB(_leds[i], _ledSegment[1].first, CRGB::Yellow, _ledSegment[_segmentTotal-1].last, CRGB::Blue );  
    fadeToBlackBy( _leds[i], _ledNumPerStrip, 30);     
    fill_gradient_RGB(_leds[i], _ledSegment[1].first, CRGB::Yellow, _topLed, CRGB::Blue );
    //_leds[i][0] = CRGB::Black;                                     // hack
  }
}

void mode3() {
  //name is day!
  for(int i = 0; i < _ledNumOfStrips; i++) {
    //fill_solid( leds[i], _ledNumPerStrip, CRGB(255, 255, 0));
    //fill_gradient_RGB(_leds[i], _ledSegment[1].first, CRGB::Green, _ledSegment[_segmentTotal-1].last, CRGB::Yellow );
    fadeToBlackBy( _leds[i], _ledNumPerStrip, 30); 
    fill_gradient_RGB(_leds[i], _ledSegment[1].first, CRGB::Green, _topLed, CRGB::Yellow );
    //_leds[i][0] = CRGB::Black;                                     // hack
  }
}

void mode4() {
  //name is working!
  //full-bright
  //isStatic should be true
  for(int i = 0; i < _ledNumOfStrips; i++) {
    //fill_solid( _leds[i], _ledNumPerStrip, CRGB::White);     // ??? _topLed
    fadeToBlackBy( _leds[i], _ledNumPerStrip, 30); 
    fill_gradient_RGB(_leds[i], _ledSegment[1].first, CRGB::White, _topLed, CRGB::White );
    //_leds[i][0] = CRGB::Black;                                     // hack
  }
  
//add sub temperature modes later..
/*
  if(_mode0_sub == 0) {
    FastLED.setTemperature( TEMPERATURE_0 ); //set first temperature  //..moved to setup ..might work ???
    //fill_solid( leds, _ledNum, TEMPERATURE_0 );
    leds[0] = TEMPERATURE_0; //show indicator pixel
  } else if(_mode0_sub == 1) {
    FastLED.setTemperature( TEMPERATURE_1 );
    //fill_solid( leds, _ledNum, TEMPERATURE_1 );
    leds[0] = TEMPERATURE_1;
  } else if(_mode0_sub == 2) {
    FastLED.setTemperature( TEMPERATURE_2 );
    //fill_solid( leds, _ledNum, TEMPERATURE_2 );
    leds[0] = TEMPERATURE_2;
  }
*/

}

void mode5() {
  //name is evening!
  for(int i = 0; i < _ledNumOfStrips; i++) {
    //fill_solid( _leds[i], _ledNumPerStrip, CRGB(128, 64, 64));
    fadeToBlackBy( _leds[i], _ledNumPerStrip, 30); 
    fill_gradient_RGB(_leds[i], _ledSegment[1].first, CRGB(128, 64, 64), _topLed, CRGB(128, 64, 64));
    //_leds[i][0] = CRGB::Black;                                     // hack
  }
}

//mode6 - name is sunset! see 'sunRiseSet'

void mode7() {
  //name is night!
  //top fade down
  for(int i = 0; i < _ledNumOfStrips; i++) {
    //fill_gradient_RGB(leds[i], _ledSegment[_segmentTotal-1].last, CRGB::White, _ledSegment[1].first, CRGB::Black );
    fadeToBlackBy( _leds[i], _ledNumPerStrip, 30); 
    fill_gradient_RGB(_leds[i], _topLed, CRGB::White, _ledSegment[1].first, CRGB::Black );
    //_leds[i][0] = CRGB::Black;                                     // hack
  }
}

void mode8() {
  //name is effect!
//  for(int i = 0; i < _ledNumOfStrips; i++) {
//    fill_solid( _leds[i], _ledNumPerStrip, CRGB(0, 0, 255));
//  }
  /* String _effectName[_effectNum] = { "Fire2012", "Confetti", "AddGlitter", "Rainbow", "RainbowWithGlitter", "Rain" }; */
  if (_effectCur == 0) {
    fire2012();
  } else if (_effectCur == 1) {
    confetti();
  } else if (_effectCur == 2) {
    glitter(80);
  } else if (_effectCur == 3) {
    rainbow();
  } else if (_effectCur == 4) {
    rainbowWithGlitter();
  } else if (_effectCur == 5) {
    rain();
  } else if (_effectCur == 6) {
    solid();
  }
}
