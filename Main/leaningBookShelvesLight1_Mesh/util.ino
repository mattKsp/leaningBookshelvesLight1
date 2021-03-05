/*----------------------------util----------------------------*/

/*
 * Mode utils
 */
void incrementPresetSlot() {
  _modePresetSlotCur += 1;
  incDecPresetSlot_prtB();
}
void decrementPresetSlot() {
  _modePresetSlotCur -= 1;
  incDecPresetSlot_prtB();
}
void incDecPresetSlot_prtB() {
  if(_modePresetSlotCur >= _modePresetSlotNum || _modePresetSlotCur < 0) { 
    _modePresetSlotCur = 0;                     //TEMP rollover catch
  }
  _modeCur = _modePreset[_modePresetSlotCur]; // set the current mode from the preset list.
}

/*
 * Color temperature utils
 */
void setColorTemp(int i) {
  _colorTempCur = i;
}
void cycleColorTemp() {
  _colorTempCur += 1;
  if (_colorTempCur >= _colorTempNum) { _colorTempCur = 0; }  // rollover
}

/*
 * Mode/ Effect utils
 */
void setEffect(int i) {
  if (i < 0) { _effectCur = 0; }
  else if (i >= _effectNum) { _effectCur = _effectNum - 1; }
  else { _effectCur = i; }
}
void cycleEffect() {
  _effectCur += 1;
  if (_effectCur >= _effectNum) { _effectCur = 0; }  // rollover
}

/*
 * Coverage mask utils
 */
void setCoverage(int i) {
  if (i < 0) { _coverageCur = 0; }
  else if (i >= _coverageNum) { _coverageCur = _coverageNum - 1; }
  else { _coverageCur = i; }
}
void cycleCoverage() {
  _coverageCur += 1;
  if (_coverageCur >= _coverageNum) { _coverageCur = 0; }  // rollover
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
    
    for(int j = 1; j < _segmentTotal; j++) {
      leds[i][ledSegment[j].first] = CRGB(i*20, j*50, 0);
      leds[i][ledSegment[j].last] = CRGB(0, j*50, i*20);
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
  leds[3][ledSegment[6].first+1] = _colorTempCur;                  //show indicator pixel
  //leds[0] = TEMPERATURE_1;                  //show indicator pixel
}

/*
 * Print digits.
 * Helper for Debug output.
 */
void printDigits(int digits) 
{
  if (DEBUG_GEN && DEBUG_COMMS) {
  Serial.print(":");
  if (digits < 10) { Serial.print('0'); }
  Serial.print(digits);
  }
}

void doReset() {
  
}

void doRestart(uint8_t restartTime) {
  
}

void doLockdown(uint8_t severity) {
  
}
