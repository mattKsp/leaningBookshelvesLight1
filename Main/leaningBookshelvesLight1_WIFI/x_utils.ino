/*----------------------------utils----------------------------*/
/*
 * Debug utils
 */
void loopDebug() {
  if (DEBUG_OVERLAY) {
    checkSegmentEndpoints();
    showColorTempPx();
  }
}

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
 * System utils
 */
void doReset() { }

void doRestart(uint8_t restartTime) { }

void doLockdown(uint8_t severity) { }
