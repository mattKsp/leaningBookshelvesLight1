/*----------------------------messages - receive----------------------------*/
void receiveMessage(char* p_topic, byte* p_payload, unsigned int p_length) {
  
  if (USE_SERIAL && DEBUG_COMMS) { Serial.println(F("mqttCallback - receive message")); }
  // concat the payload into a string
  String payload;
  for (uint8_t i = 0; i < p_length; i++) {
    payload.concat((char)p_payload[i]);
  }
  
  // handle message topics
  if (String(MQTT_LIGHTS_TOPIC_COMMAND).equals(p_topic)) { receivedSwitch(payload); } 
  if (String(MQTT_LIGHTS_BRIGHTNESS_TOPIC_COMMAND).equals(p_topic)) { receivedBrightness(payload); } 
  if (String(MQTT_LIGHTS_MODE_TOPIC_COMMAND).equals(p_topic)) { receivedMode(payload); } 
  if (String(MQTT_LIGHTS_MODE_COLTEMP_TOPIC_COMMAND).equals(p_topic)) { receivedModeColtemp(payload); }  
  if (String(MQTT_LIGHTS_MODE_EFFECT_TOPIC_COMMAND).equals(p_topic)) { receivedModeEffect(payload); }  
  if (String(MQTT_LIGHTS_MODE_COVERAGE_TOPIC_COMMAND).equals(p_topic)) { receivedModeCoverage(payload); }  
  
  if (String(MQTT_LIGHTS_SUNRISE_TOPIC_COMMAND).equals(p_topic)) { receivedSunrise(payload); }  
  if (String(MQTT_SUNRISE_GLOBAL_TOPIC_COMMAND).equals(p_topic)) { receivedSunriseGlobal(payload); } 
  if (String(MQTT_LIGHTS_SUNSET_TOPIC_COMMAND).equals(p_topic)) { receivedSunset(payload); } 
  if (String(MQTT_SUNSET_GLOBAL_TOPIC_COMMAND).equals(p_topic)) { receivedSunsetGlobal(payload); } 
  
  if (String(MQTT_LIGHTS_BREATH_TOPIC_COMMAND).equals(p_topic)) { receivedBreath(payload); } 
  if (String(MQTT_BREATH_GLOBAL_TOPIC_COMMAND).equals(p_topic)) { receivedBreathGlobal(payload); } 
  if (String(MQTT_LIGHTS_BREATH_XYZ_TOPIC_COMMAND).equals(p_topic)) { receivedBreathXyz(payload); } 
  if (String(MQTT_LIGHTS_BREATH_XYZ_MODE_TOPIC_COMMAND).equals(p_topic)) { receivedBreathXyzMode(payload); } 
  
  if (String(MQTT_DEBUG_GENERAL_TOPIC_COMMAND).equals(p_topic)) { receivedDebugGeneral(payload); } 
  if (String(MQTT_DEBUG_OVERLAY_TOPIC_COMMAND).equals(p_topic)) { receivedDebugOverlay(payload); } 
  if (String(MQTT_DEBUG_COMMS_TOPIC_COMMAND).equals(p_topic)) { receivedDebugComms(payload); } 
  if (String(MQTT_DEBUG_RESET_TOPIC_COMMAND).equals(p_topic)) { receivedDebugReset(payload); } 
  if (String(MQTT_DEBUG_RESTART_TOPIC_COMMAND).equals(p_topic)) { receivedDebugRestart(payload); } 
  
  if (String(MQTT_RESET_GLOBAL_TOPIC_COMMAND).equals(p_topic)) { receivedResetGlobal(payload); } 
  if (String(MQTT_RESTART_GLOBAL_TOPIC_COMMAND).equals(p_topic)) { receivedRestartGlobal(payload); } 
  if (String(MQTT_LOCKDOWN_GLOBAL_TOPIC_COMMAND).equals(p_topic)) { receivedLockdown(payload); } 
  if (String(MQTT_STATUS_REQUEST_TOPIC_COMMAND).equals(p_topic)) { receivedStatusRequest(payload); } 

  if (USE_SERIAL && DEBUG_COMMS) { Serial.print(p_topic); Serial.print(F(" - ")); Serial.println(payload); }
}

// message topics
void receivedSwitch(String payload) {
  if (payload.equals(String(LIGHTS_ON))) { _onOff = true; } 
  else if (payload.equals(String(LIGHTS_OFF))) { _onOff = false; }
  publishState(true);
}

void receivedBrightness(String payload) {
  uint8_t brightness = payload.toInt();
  if (brightness < 0 || brightness > 255) { /* do nothing */ return; } 
  else {
    setGlobalBrightness(brightness);
    publishBrightness(true);
  }
}

void receivedMode(String payload) {
  if (payload == "Glow")         { _modeCur = 0; _modePresetSlotCur = 0; } /*= _modePreset[0];*/ 
//else if (payload == "Sunrise") { _modeCur = 1; }
  else if (payload == "Morning") { _modeCur = 2; _modePresetSlotCur = 1; }
  else if (payload == "Day")     { _modeCur = 3; _modePresetSlotCur = 2; }
  else if (payload == "Working") { _modeCur = 4; _modePresetSlotCur = 3; }
  else if (payload == "Evening") { _modeCur = 5; _modePresetSlotCur = 4; }
//else if (payload == "Sunset")  { _modeCur = 6; }
  else if (payload == "Night")   { _modeCur = 7; _modePresetSlotCur = 5; }
  else if (payload == "Effect")  { _modeCur = 8; }
  else { }

  _modeString = payload; // redundant ???
  publishMode(true);
}
// void receivedSubMode() { }

void receivedModeColtemp(String payload) {
  if      (payload == "Warm")     { setColorTemp(0); }
  else if (payload == "Standard")  { setColorTemp(1); }
  else if (payload == "CoolWhite") { setColorTemp(2); }
  
  publishModeColorTemp(true);
}

void receivedModeEffect(String payload) {
  /* String _effectName[_effectNum] = { "Fire2012", "Confetti", "AddGlitter", "Rainbow", "RainbowWithGlitter", "Rain" }; */
  if      (payload == "Fire2012")           { setEffect(0); }
  else if (payload == "Confetti")           { setEffect(1); }
  else if (payload == "AddGlitter")         { setEffect(2); }
  else if (payload == "Rainbow")            { setEffect(3); }
  else if (payload == "RainbowWithGlitter") { setEffect(4); }
  
  publishModeEffect(true);
}

void receivedModeCoverage(String payload) {
  /* String _coverageName[_coverageNum] = {"Full", "HiCut", "LowCut", "HiEdgeCut", "FullEdgeCut", "BackProfile" }; */
  if      (payload == "Full")         { setCoverage(0); }
  else if (payload == "HiCut")        { setCoverage(1); }
  else if (payload == "LowCut")       { setCoverage(2); }
  else if (payload == "HiEdgeCut")    { setCoverage(3); }
  else if (payload == "FullEdgeCut")  { setCoverage(4); }
  else if (payload == "BackProfile")  { setCoverage(5); }
  
  publishModeCoverage(true);
}

/* Sunset and Sunrise */
void receivedSunrise(String payload) {
  if (payload.equals(String(LIGHTS_ON))) { /* start sunrise */ } 
  else if (payload.equals(String(LIGHTS_OFF))) { /* stop sunrise and revert to previous setting */ }
}

void receivedSunriseGlobal(String payload) {
  if (payload.equals(String(LIGHTS_ON))) { /* start sunrise */ } 
  else if (payload.equals(String(LIGHTS_OFF))) { /* stop sunrise and revert to previous setting */ }
}

void receivedSunset(String payload) {
  if (payload.equals(String(LIGHTS_ON))) { /* start sunset */ } 
  else if (payload.equals(String(LIGHTS_OFF))) { /* stop sunset and revert to previous setting */ }
}

void receivedSunsetGlobal(String payload) {
  if (payload.equals(String(LIGHTS_ON))) { /* start sunset */ } 
  else if (payload.equals(String(LIGHTS_OFF))) { /* stop sunset and revert to previous setting */ }
}

/* Breath */
void receivedBreath(String payload) {
  if (payload.equals(String(ON))) {
    _isBreathingSynced = false;                   // set sync to local
    _isBreathing = true;                          // start local breathing
  }
  else if (payload.equals(String(OFF))) {
    _isBreathing = false;                         // stop breathing
    _isBreathingSynced = false;                   // set sync to local
  }
}

void receivedBreathGlobal(String payload) {
  if (payload.equals(String(ON))) {
    _isBreathingSynced = true;                    // set sync to global
    _isBreathing = true;                          // start synced breathing
  }
  else if (payload.equals(String(OFF))) {
    _isBreathing = false;                         // stop breathing
    _isBreathingSynced = false;                   // set sync to local
  }
}

void receivedBreathXyz(String payload) {
  // msg will contain xyz coords for position within the house
}

void receivedBreathXyzMode(String payload) {
  // set positional mode to independent or global
  if (payload == "Independent") { }
  else if (payload == "Global") { }
}

/* Debug */
void receivedDebugGeneral(String payload) {
  if (payload.equals(String(ON))) { DEBUG_GEN = true; }
  else if (payload.equals(String(OFF))) { DEBUG_GEN = false; }
  publishDebugGeneralState(false);
}

void receivedDebugOverlay(String payload) {
  if (payload.equals(String(ON))) { DEBUG_OVERLAY = true; }
  else if (payload.equals(String(OFF))) { DEBUG_OVERLAY = false; }
    publishDebugOverlayState(false);
}

void receivedDebugComms(String payload) {
  if (payload.equals(String(ON))) { DEBUG_COMMS = true; }
  else if (payload.equals(String(OFF))) { DEBUG_COMMS = false; }
  publishDebugCommsState(false);
}

void receivedDebugReset(String payload) {
  if (payload.equals(String(ON))) { doReset(); }
}

void receivedDebugRestart(String payload) {
  uint8_t restartTime = payload.toInt();
  if (restartTime < 0 || restartTime > 255) { /* do nothing */ return; } 
  else { doRestart(restartTime); }
}

/* System */
void receivedResetGlobal(String payload) {
  if (payload.equals(String(ON))) { doReset(); }
}

void receivedRestartGlobal(String payload) {
  uint8_t restartTime = payload.toInt();
  if (restartTime < 0 || restartTime > 255) { /* do nothing */ return; } 
  else { doRestart(restartTime); }
}

void receivedLockdown(String payload) {
  uint8_t severity = payload.toInt();
  if (severity < 0 || severity > 255) { /* do nothing */ return; } 
  else { doLockdown(severity); }
}

void receivedStatusRequest(String payload) {
  if (payload.equals(String(ON))) { publishStatusAll(false); }
}
