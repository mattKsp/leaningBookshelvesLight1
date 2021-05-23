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
 * System utils
 */
void doReset() { }

void doRestart(uint8_t restartTime) { }

void doLockdown(uint8_t severity) { }
