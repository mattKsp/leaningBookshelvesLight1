/*----------------------------messages - receive----------------------------*/
void receiveMessage(uint32_t from, String msg)
{
  uint8_t firstMsgIndex = msg.indexOf(':');
  String targetSub = msg.substring(0, firstMsgIndex);
  String msgSub = msg.substring(firstMsgIndex+1);

  if (targetSub == "lights/light/switch")
  {
    if (msgSub == LIGHTS_ON) { _onOff = true; }
    else if (msgSub == LIGHTS_OFF) { _onOff = false; }
    publishState(true);
  } 
  else if (targetSub == "lights/brightness/set")
  {
    uint8_t brightness = msgSub.toInt();
    if (brightness < 0 || brightness > 255) {
      // do nothing...
      return;
    } else {
      setGlobalBrightness(brightness);
      publishBrightness(true);
    }
  }
  else if (targetSub == "lights/mode")
  {
    if (msgSub == "Glow")         { _modeCur = 0; _modePresetSlotCur = 0; } /*= _modePreset[0];*/ 
  //else if (msgSub == "Sunrise") { _modeCur = 1; }
    else if (msgSub == "Morning") { _modeCur = 2; _modePresetSlotCur = 1; }
    else if (msgSub == "Day")     { _modeCur = 3; _modePresetSlotCur = 2; }
    else if (msgSub == "Working") { _modeCur = 4; _modePresetSlotCur = 3; }
    else if (msgSub == "Evening") { _modeCur = 5; _modePresetSlotCur = 4; }
  //else if (msgSub == "Sunset")  { _modeCur = 6; }
    else if (msgSub == "Night")   { _modeCur = 7; _modePresetSlotCur = 5; }
    else if (msgSub == "Effect")  { _modeCur = 8; }
    else { }

    _modeString = msgSub; // redundant ???
    //publishMode(true);
  }
//  else if (targetSub == "lights/mode/coltemp")
//  {
//    if      (msgSub == "Warm")      { setColorTemp(0); }
//    else if (msgSub == "Standard")  { setColorTemp(1); }
//    else if (msgSub == "CoolWhite") { setColorTemp(2); }
//    
//    publishColorTemp(true);
//  }
  else if (targetSub == "lights/mode/effect")
  {
    /* String _effectName[_effectNum] = { "Fire2012", "Confetti", "AddGlitter", "Rainbow", "RainbowWithGlitter", "Rain" }; */
    if      (msgSub == "Fire2012")            { setEffect(0); }
    else if (msgSub == "Confetti")            { setEffect(1); }
    else if (msgSub == "AddGlitter")          { setEffect(2); }
    else if (msgSub == "Rainbow")             { setEffect(3); }
    else if (msgSub == "RainbowWithGlitter")  { setEffect(4); }

    publishEffect(true);
  }
  else if (targetSub == "lights/mode/coverage")
  {
    /* String _coverageName[_coverageNum] = {"Full", "HiCut", "LowCut", "HiEdgeCut", "FullEdgeCut", "BackProfile" }; */
    if      (msgSub == "Full")                { setCoverage(0); }
    else if (msgSub == "HiCut")               { setCoverage(1); }
    else if (msgSub == "LowCut")              { setCoverage(2); }
    else if (msgSub == "HiEdgeCut")           { setCoverage(3); }
    else if (msgSub == "FullEdgeCut")         { setCoverage(4); }
    else if (msgSub == "BackProfile")         { setCoverage(5); }

    //publishCoverage(true);
  }
  else if (targetSub == "sunrise")
  {
    // trigger only (global synced)
    if (msgSub == LIGHTS_ON) {
      //start sunrise
    }
    else if (msgSub == LIGHTS_OFF) {
      //stop sunrise and revert to previous setting
    }
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.println(msgSub); }
  }
  else if (targetSub == "lights/sunrise") {
    // trigger only
    // note: the single mesh msg of 'sunrise' is used for synced global sunrise
    if (msgSub == LIGHTS_ON) {
      //start sunrise
    }
    else if (msgSub == LIGHTS_OFF) {
      //stop sunrise and revert to previous setting
      //publishMode(true);
    }
    //else if (msgSub == "receive a time for sunrise to happen at") {
    //set sunrise time
    //}
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.println(msgSub); }
  }
  else if (targetSub == "sunset")
  {
    // trigger only (global synced)
    if (msgSub == LIGHTS_ON) {
      //start sunset
    }
    else if (msgSub == LIGHTS_OFF) {
      //stop sunset and revert to previous setting
    }
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.println(msgSub); }
  }
  else if (targetSub == "lights/sunset") {
    // trigger only
    // note: the single mesh msg of 'sunset' is used for synced global sunset
    if (msgSub == LIGHTS_ON) {
      //start sunset
    }
    else if (msgSub == LIGHTS_OFF) {
      //stop sunset and revert to previous setting
    }
    //else if (msgSub == "receive a time for sunset to happen at") {
    //set sunset time
    //}
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.println(msgSub); }
  }
  /*
   * Breath : (noun) Refers to a full cycle of breathing. It can also refer to the air that is inhaled or exhaled.
   */
  else if (targetSub == "breath")
  {
    // trigger only (global synced)
    // note: the single mesh msg of 'breath' is used for synced global breathing
    if (msgSub == ON) {
      _isBreathingSynced = true;                    // set sync to global
      _isBreathing = true;                          // start synced breathing
    }
    else if (msgSub == OFF) {
      _isBreathing = false;                         // stop breathing
      _isBreathingSynced = false;                   // set sync to local
    }
    //publishBreath(false);
  }
  else if (targetSub == "lights/breath")
  {
    // trigger only (local)
    // note: the single mesh msg of 'breath' is used for synced global breathing
    if (msgSub == ON) {
      _isBreathingSynced = false;                   // set sync to local
      _isBreathing = true;                          // start local breathing
    }
    else if (msgSub == OFF) {
      _isBreathing = false;                         // stop breathing
      _isBreathingSynced = false;                   // set sync to local
    }
    //publishLightsBreath(false);
  }
  else if (targetSub == "lights/breath/xyz")
  {
    // msg will contain xyz coords for position within the house
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.println(msgSub); }
  }
  else if (targetSub == "lights/breath/xyz/mode")
  {
    // set positional mode
    // independent, global
    if (msgSub == "Independent") { }
    else if (msgSub == "Global") { }
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.println(msgSub); }
  }
  else if(targetSub == "debug/general/set") 
  { 
    if (msgSub == ON) { DEBUG_GEN = true; } 
    else if (msgSub == OFF) { DEBUG_GEN = false; }
    publishDebugGeneralState(false);
  }
  else if (targetSub == "debug/overlay/set")
  {
    if (msgSub == ON) { DEBUG_OVERLAY = true; }
    else if (msgSub == OFF) { DEBUG_OVERLAY = false; }
    publishDebugOverlayState(false);
  }
  else if (targetSub == "debug/meshsync/set")
  {
    if (msgSub == ON) { DEBUG_MESHSYNC = true; }
    else if (msgSub == OFF) { DEBUG_MESHSYNC = false; }
    publishDebugMeshsyncState(false);
  }
  else if(targetSub == "debug/comms/set") 
  {
    if (msgSub == ON) { 
      DEBUG_COMMS = true;
      if (!Serial){ Serial.begin(115200); }
      } 
    else if (msgSub == OFF) { 
      DEBUG_COMMS = false;
      if (Serial) { Serial.end(); }
      }
    publishDebugCommsState(false);
  }
  // don't really need an ON msg but using just to sure it wasn't sent in error
  else if(targetSub == "debug/reset") { if (msgSub == ON) { doReset(); } }
  else if(targetSub == "debug/restart") 
  {
    uint8_t restartTime = msg.toInt();
    if (restartTime < 0 || restartTime > 255) { return; /* do nothing... */ } 
    else { doRestart(restartTime); }
  }
  else if(targetSub == "reset") { if (msgSub == ON) { doReset(); } }
  else if(targetSub == "restart") 
  {
    uint8_t restartTime = msg.toInt();
    if (restartTime < 0 || restartTime > 255) { return; /* do nothing... */ } 
    else { doRestart(restartTime); }
  }
  else if(targetSub == "lockdown") 
  {
    uint8_t severity = msg.toInt();
    if (severity < 0 || severity > 255) { return; /* do nothing... */ } 
    else { doLockdown(severity); }
  }
  else if(targetSub == "status/request") { if (msgSub == ON) { publishStatusAll(false); }  }
  
  if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
}
