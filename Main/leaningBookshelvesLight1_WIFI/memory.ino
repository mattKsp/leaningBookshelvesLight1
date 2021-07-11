/*----------------------------memory----------------------------*/

void loadSettings() {
  preferences.begin("lightcontrol", true);  // true = read-only

  int isInit = preferences.getInt("isinit", 0); // have the settings been initialised?
  if (isInit == 0) {
    preferences.end();
    saveSettings();
  } else {
    _ledGlobalBrightnessCur = preferences.getInt("globalBrightCur", 255);
    
    preferences.end();
  }
}

void saveSettings() {
  preferences.begin("lightcontrol", false); // false = read/write

  preferences.putInt("isinit", 1);
  preferences.putInt("globalBrightCur", _ledGlobalBrightnessCur);
  
  preferences.end();
}

void clearSettings() {
  preferences.begin("lightcontrol", false); // false = read/write
  preferences.clear();
  preferences.end();
}

void loopSaveSettings() {
  EVERY_N_SECONDS(60) {                       // too much ???
    if (_shouldSaveSettings == true)
    { 
      saveSettings(); 
      _shouldSaveSettings = false; 
    }
  }
}

void resetDefaults() {
  clearSettings();
  setDefaults();
  saveSettings();
}

void setDefaults() 
{
  DEBUG_GEN = false;
  DEBUG_OVERLAY = false;
  DEBUG_MESHSYNC = false;
  DEBUG_COMMS = false;
  DEBUG_USERINPUT = false;
  DEBUG_TIME = false;

  _firstTimeSetupDone = false;
  _onOff = false;
  
  _isBreathing = false;
  _isBreathOverlaid = false;
  _isBreathingSynced = false; 

  _modeCur = 0;
  _modePresetSlotCur = 0;
  _colorTempCur = 1;

  _ledGlobalBrightnessCur = _ledGlobalBrightness;
  _gHue = 0;
}
