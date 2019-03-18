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
      fadeToBlackBy( leds[i], _ledNumPerStrip, 30); 
    } 
  } //END onOff else
}

/*----------------------------modes----------------------------*/
void mode0() {
  //name should be glow!
  //bot fade up
  for(int i = 0; i < _ledNumOfStrips; i++) {
    fill_gradient_RGB(leds[i], ledSegment[0].first, CRGB(32, 32, 32), ledSegment[_segmentTotal-1].last, CRGB::Black );
  }
}

//mode1 - see 'sunRiseSet'

void mode2() {
  //name is morning!
  for(int i = 0; i < _ledNumOfStrips; i++) {
    //fill_solid( leds[i], _ledNumPerStrip, CRGB(0, 0, 255));
    fill_gradient_RGB(leds[i], ledSegment[0].first, CRGB::Yellow, ledSegment[_segmentTotal-1].last, CRGB::Blue );
  }
}

void mode3() {
  //name is day!
  for(int i = 0; i < _ledNumOfStrips; i++) {
    //fill_solid( leds[i], _ledNumPerStrip, CRGB(255, 255, 0));
    //fill_gradient_RGB(leds[i], ledSegment[0].first, CRGB::Green, ledSegment[_segmentTotal/2].first, CRGB::Blue, ledSegment[_segmentTotal-1].last, CRGB::Yellow );
    fill_gradient_RGB(leds[i], ledSegment[0].first, CRGB::Green, ledSegment[_segmentTotal-1].last, CRGB::Yellow );
  }
}

void mode4() {
  //name is working!
  //full-bright
  //isStatic should be true
  for(int i = 0; i < _ledNumOfStrips; i++) {
    fill_solid( leds[i], _ledNumPerStrip, CRGB::White);
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
    fill_solid( leds[i], _ledNumPerStrip, CRGB(128, 64, 64));
  }
}

//mode6 - see 'sunRiseSet'

void mode7() {
  //name is night!
  //top fade down
  for(int i = 0; i < _ledNumOfStrips; i++) {
    fill_gradient_RGB(leds[i], ledSegment[_segmentTotal-1].last, CRGB::White, ledSegment[0].first, CRGB::Black );
  }
}

void mode8() {
  //name is changing!
//  for(int i = 0; i < _ledNumOfStrips; i++) {
//    fill_solid( leds[i], _ledNumPerStrip, CRGB(0, 0, 255));
//  }
}


