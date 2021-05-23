/*----------------------------messages - publish-----------------------*/

//  ...this is gonna complain about lengths of char  ..so prob have to do it the long way..... !!!

void meshSendSingleToServer(char* nom, char* msg, bool save) {
  if (DEBUG_COMMS) { Serial.print(nom); Serial.print(" - "); }
    mqttClient.publish(nom, msg, true);
  if (DEBUG_COMMS) { Serial.println(msg); }
  if (save == true) { _shouldSaveSettings = true; }
}
//void publishMeshMsgSingleState(String nom, String addr, boolean state, bool save) {
void publishMeshMsgSingleState(char* nom, char* addr, boolean state, bool save) {
  if (state == false) {  addr = LIGHTS_OFF; }
  else if (state == true) { addr = LIGHTS_ON; }
  meshSendSingleToServer(nom, addr, save);
}
void publishMeshMsgSingleString(char* nom, char* addr, char* msg, bool save) {
  
}
void publishMeshMsgSingleColor(char* nom, char* addr, uint8_t r, uint8_t g, uint8_t b, bool save) {
  
}

/*----------------------------messages - publish - main----------------*/
void publishState(bool save) {
  publishMeshMsgSingleState("publishState", "house/leaningbookshelves1/lights/light/status", _onOff, save);
}

void publishBrightness(bool save) {
//  publishMeshMsgSingleString("publishBrightness", "lights/brightness/status", String(_ledGlobalBrightnessCur), save);
}

void publishMode(bool save) {
//  publishMeshMsgSingleString("publishMode", "lights/mode", _modeName[_modeCur], save);
}

void publishSubMode(bool save) { /* TODO - but might just use ColTemp */ }

//void publishColorTemp(bool save) {
//  publishMeshMsgSingleString("publishColorTemp", "lights/mode/coltemp", _colorTempName[_colorTempCur], save);
//}

void publishEffect(bool save)
{
  /* String _effectName[_effectNum] = { "Fire2012", "Confetti", "AddGlitter", "Rainbow", "RainbowWithGlitter", "Rain" }; */
//  publishMeshMsgSingleString("publishEffect", "lights/mode/effect", _effectName[_effectCur], save);
}

void publishCoverage(bool save)
{
  /* String _coverageName[_coverageNum] = {"Full", "HiCut", "LowCut", "HiEdgeCut", "FullEdgeCut", "BackProfile" }; */
//  publishMeshMsgSingleString("publishCoverage", "lights/mode/coverage", _coverageName[_coverageCur], save);
}

void publishDebugGeneralState(bool save)
{
//  publishMeshMsgSingleState("publishDebugGeneralState", "debug/general/status", DEBUG_GEN, save);
}

void publishDebugOverlayState(bool save)
{
//  publishMeshMsgSingleState("publishDebugOverlayState", "debug/overlay/status", DEBUG_OVERLAY, save);
}

void publishDebugMeshsyncState(bool save)
{
//  publishMeshMsgSingleState("publishDebugMeshsyncState", "debug/meshsync/status", DEBUG_MESHSYNC, save);
}

void publishDebugCommsState(bool save)
{
//  publishMeshMsgSingleState("publishDebugCommsState", "debug/comms/status", DEBUG_COMMS, save);
}

void publishStatusAll(bool save) 
{
  if (DEBUG_COMMS) { Serial.println("publishStatusAll "); }
  //
}
