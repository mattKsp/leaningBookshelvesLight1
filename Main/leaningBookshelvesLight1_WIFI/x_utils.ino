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
void turnOffWifi() {
  if (DEBUG_GEN && Serial) { Serial.println("Disconnecting wifi..."); }
  WiFi.disconnect();
}

void turnOffSerial() {
  if (DEBUG_GEN) { Serial.println("Disconnecting serial..."); }
  //
}

void factoryReset() { /* TODO */ }

void deviceRestart() { ESP.restart(); }

void deviceShutdown() { /* ??? */ }

/*----------------------------main calls-----------------------*/
void doReset() { 
  resetDefaults();
  deviceRestart();
}

void doRestart(uint8_t restartTime) {
  uint16_t dly = (restartTime * 60 * 1000); // static ???
  delay(dly);
  deviceRestart();
}

/*
 * Lockdown.
 * 
 * Emergency global disconnect (requires hard reset). 
 *  0 = do thing (no severity)
 *  1 = disconnect from LAN (bridges shutdown and mesh reboots) 
 *  2 = shutdown mesh (everything reboots in standalone mode) 
 *  3 = shutdown everything. 
 *      - Devices to power off if possible, if not then reboot in standalone emergency mode. 
 *      - These devices will require a hardware reset button implemented to clear the emergency mode.)
 */
void doLockdown(uint8_t severity) { }
