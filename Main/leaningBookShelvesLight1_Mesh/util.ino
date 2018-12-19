/*----------------------------util----------------------------*/

/*
 * Mode utils
 */
void incrementPresetSlot() {
  _modePresetSlotCur += 1;
  incDecPresSlot_prtB();
}
void decrementPresetSlot() {
  _modePresetSlotCur -= 1;
  incDecPresSlot_prtB();
}
void incDecPresSlot_prtB() {
  if(_modePresetSlotCur >= _modePresetSlotNum){ _modePresetSlotCur = 0; }  //TEMP rollover catch
  _modeCur = _modePreset[_modePresetSlotCur];
}

/*
 * Golbal brightness utils
 */
void setGlobalBrightness(int gb) {
  //use this to achieve an override from the mesh, eg. to match levels
  _ledGlobalBrightnessCur = gb;
  brightnessRolloverCatch();
}
void increaseBrightness() {
  _ledGlobalBrightnessCur += _ledBrightnessIncDecAmount;
  brightnessRolloverCatch();
}
void decreaseBrightness() {
  _ledGlobalBrightnessCur -= _ledBrightnessIncDecAmount;
  brightnessRolloverCatch();
}
void brightnessRolloverCatch() {
  if(_ledGlobalBrightnessCur > 255) {
    _ledGlobalBrightnessCur = 255;
  } else if(_ledGlobalBrightnessCur < 0) {
    _ledGlobalBrightnessCur = 0;
  }
}

/*
 Check Segment Endpoints
 usage | checkSegmentEndpoints();
 overlay/place near end of chain
 This places different coloured lights at the ends of the LED strip(s) segments for quick visual feedback of calculations
 */
void checkSegmentEndpoints() {
  
  for(int i = 0; i < _ledNumOfStrips; i++) {
    fill_solid( leds[i], _ledNumPerStrip, CRGB(0,0,0));
    
    for(int j = 0; j < _segmentTotal; j++) {
      leds[i][ledSegment[_segmentTotal].first] = CRGB(i*20, j*50, 0);
      leds[i][ledSegment[_segmentTotal].last] = CRGB(0, j*50, i*20);
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
  leds[0][0] = _colorTempCur;                  //show indicator pixel
  //leds[0] = TEMPERATURE_1;                  //show indicator pixel
}

