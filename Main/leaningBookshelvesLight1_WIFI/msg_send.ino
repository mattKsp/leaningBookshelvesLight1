/*----------------------------messages - publish-----------------------*/

//  ...this is gonna complain about lengths of char  ..so prob have to do it the long way..... !!!

void meshSendSingleToServer(String nom, char* addr, char* msg, bool save) {
  if (USE_SERIAL && DEBUG_COMMS) { Serial.print(nom); Serial.print(F(" - ")); }
    mqttClient.publish(addr, msg, true);
  if (USE_SERIAL && DEBUG_COMMS) { Serial.println(msg); }
  if (save == true) { _shouldSaveSettings = true; }
}
void publishMeshMsgSingleState(String nom, char* addr, boolean state, bool save) {
  char* msg = LIGHTS_OFF;
  if (state == true) { msg = LIGHTS_ON; }
  snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%s", msg);
  meshSendSingleToServer(nom, addr, m_msg_buffer, save);
}
void publishMeshMsgSingleString(String nom, char* addr, String msg, bool save) {
  snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%s", msg);
  meshSendSingleToServer(nom, addr, m_msg_buffer, save);
}
void publishMeshMsgSingleColor(String nom, char* addr, uint8_t r, uint8_t g, uint8_t b, bool save) {
  String msg = String(r);
  msg += ",";
  msg += String(g);
  msg += ",";
  msg += String(b);
  snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%s", msg);
  meshSendSingleToServer(nom, addr, m_msg_buffer, save);
}

/*----------------------------messages - publish - main----------------*/
void publishState(bool save) {
  publishMeshMsgSingleState("publishState", MQTT_LIGHTS_TOPIC_STATE, _onOff, save);
}

/* void publishBrightness(bool save) {
  publishMeshMsgSingleString("publishBrightness", MQTT_LIGHTS_BRIGHTNESS_TOPIC_STATE, String(_ledGlobalBrightnessCur), save);
} */

void publishGlobalBrightness(bool save) {
  publishMeshMsgSingleString("publishGlobalBrightness", MQTT_LIGHTS_GLOBALBRIGHTNESS_TOPIC_STATE, String(_ledGlobalBrightnessCur), save);
}
void publishRGB(bool save) {
  publishMeshMsgSingleColor("publishRGB", MQTT_LIGHTS_RGB_TOPIC_STATE, effectColor_RGB.red, effectColor_RGB.green, effectColor_RGB.blue, save);
}

void publishMode(bool save) {
  publishMeshMsgSingleString("publishMode", MQTT_LIGHTS_MODE_TOPIC_STATE, _modeName[_modeCur], save);
}

//void publishSubMode(bool save) { /* TODO - but might just use ColTemp */ }

void publishModeColorTemp(bool save) {
  publishMeshMsgSingleString("publishColorTemp", MQTT_LIGHTS_MODE_COLTEMP_TOPIC_STATE, _colorTempName[_colorTempCur], save);
}

void publishModeEffect(bool save) {
  /* String _effectName[_effectNum] = { "Fire2012", "Confetti", "AddGlitter", "Rainbow", "RainbowWithGlitter", "Rain" }; */
  publishMeshMsgSingleString("publishEffect", MQTT_LIGHTS_MODE_EFFECT_TOPIC_STATE, _effectName[_effectCur], save);
}

void publishModeCoverage(bool save) {
  /* String _coverageName[_coverageNum] = {"Full", "HiCut", "LowCut", "HiEdgeCut", "FullEdgeCut", "BackProfile" }; */
  publishMeshMsgSingleString("publishCoverage", MQTT_LIGHTS_MODE_COVERAGE_TOPIC_STATE, _coverageName[_coverageCur], save);
}

// passing number straight through so as not to use converted squashed number
void publishTopLed(uint8_t tl, bool save) {
  publishMeshMsgSingleString("publishTopLed", MQTT_LIGHTS_TOPLED_TOPIC_STATE, String(tl), save);
}

/* Debug */
void publishDebugGeneralState(bool save) {
  publishMeshMsgSingleState("publishDebugGeneralState", MQTT_DEBUG_GENERAL_TOPIC_STATE, DEBUG_GEN, save);
}

void publishDebugOverlayState(bool save) {
  publishMeshMsgSingleState("publishDebugOverlayState", MQTT_DEBUG_OVERLAY_TOPIC_STATE, DEBUG_OVERLAY, save);
}

void publishDebugCommsState(bool save){
  publishMeshMsgSingleState("publishDebugCommsState", MQTT_DEBUG_COMMS_TOPIC_STATE, DEBUG_COMMS, save);
}

/* System */
void publishStatusAll(bool save) {
  publishState(save);
  //publishBrightness(save);
  publishGlobalBrightness(save);
  publishRGB(save);
  publishMode(save);
//publishSubMode(save);
  publishModeColorTemp(save);
  publishModeEffect(save);
  publishModeCoverage(save);
  //publishTopLed( , save);

  publishDebugGeneralState(save);
  publishDebugOverlayState(save);
  publishDebugCommsState(save);
  
  if (USE_SERIAL && DEBUG_COMMS) { Serial.println("publishStatusAll "); }
}

void publishDeviceOffline() {
  
}
