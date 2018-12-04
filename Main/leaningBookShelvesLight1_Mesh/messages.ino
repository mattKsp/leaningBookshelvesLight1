/*----------------------------internal mesh messages - get passed to/from MQTT broker by bridge----------------------------*/
void receiveMessage(uint32_t from, String msg)
{
  uint8_t firstMsgIndex = msg.indexOf(':');
  String targetSub = msg.substring(0, firstMsgIndex);
  String msgSub = msg.substring(firstMsgIndex);

/*
  _debugOverlay = checkBool(_integer1FromSerial);
  setSunRise(_integer1FromSerial, _integer2FromSerial);             //???
  _sunRiseEnabled = true;
  //setSunSet(_integer1FromSerial, _integer2FromSerial);             //???
  _sunRiseEnabled = false;
  _sunRiseEnabled = checkBool(_integer1FromSerial);
  _sunSetEnabled = checkBool(_integer1FromSerial);
  setGlobalBrightness(_integer1FromSerial);
  _modeCur = _integer1FromSerial;
  _onOff = checkBool(_integer1FromSerial);
*/

  if (targetSub == "lights/light/switch")
  {
    if (msgSub == "ON")
    {
      _onOff = true;
    }
    else if (msgSub == "OFF")
    {
      _onOff = false;
    }
    publishState();
  } 
  else if (targetSub == "lights/brightness/set")
  {
    uint8_t brightness = msgSub.toInt();
    if (brightness < 0 || brightness > 255) {
      // do nothing...
      return;
    } else {
      setGlobalBrightness(brightness);
      publishBrightness();
    }
  }
  else if (targetSub == "lights/mode")
  {
    /*
    if (msgSub == "Glow") 
    { _modeCur = 0; }
    else if (msgSub == "Sunrise") 
    { _modeCur = 1; }
    else if (msgSub == "Morning") 
    { _modeCur = 2; }
    else if (msgSub == "Day") 
    { _modeCur = 3; }
    else if (msgSub == "Working") 
    { _modeCur = 4; }
    else if (msgSub == "Evening") 
    { _modeCur = 5; }
    else if (msgSub == "Sunset") 
    { _modeCur = 6; }
    else if (msgSub == "Night") 
    { _modeCur = 7; }
    else if (msgSub == "Effect") 
    { _modeCur = 8; }
    else { }
    */
    
    for (int i = 0; i < _modeNum; i++) {
      if (msgSub == modeName[i]) {
        _modeCur = i;
        _modeString = msgSub;
        break;
      }
    }
    
    publishMode();
  }
  else if (targetSub == "sunrise")
  {
    //this is responding to a global network message
    if (msgSub == "ON") {
      //_modeString = "Sunrise";
      //_onOff = true;
      //publishMode();
    }
  }
  else if (targetSub == "sunset")
  {
    //this is responding to a global network message
    if (msgSub == "ON") {
      //_modeString = "Sunset";
      //_onOff = true;
      //publishMode();
    }
  }
  if (DEBUG) { Serial.print(targetSub); Serial.println(msgSub); }
  
}


void publishState()
{
  if (DEBUG) { Serial.print("publishState "); }
  String msg = "lights/light/status";
  msg += ":"; //..just so we are all sure what is going on here !?
  if(_onOff == false) {
    msg += "OFF";
  } else {
    msg += "ON";
  }
  uint32_t id = DEVICE_ID_BRIDGE1;
  mesh.sendSingle(id, msg);
  //mesh.sendBroadcast(msg);
  if (DEBUG) { Serial.println(msg); }
  //shouldSaveSettings = true;
}

void publishBrightness()
{
  if (DEBUG) { Serial.print("publishBrightness "); }
  String msg = "lights/brightness/status";
  msg += ":"; //..just so we are all sure what is going on here !?
  msg += String(_ledGlobalBrightnessCur);
  uint32_t id = DEVICE_ID_BRIDGE1;
  mesh.sendSingle(id, msg);
  if (DEBUG) { Serial.println(msg); }
  //shouldSaveSettings = true;
}

void publishMode()
{
  if (DEBUG) { Serial.print("publishMode "); }
  String msg = "lights/mode";
  msg += ":"; //..just so we are all sure what is going on here !?
  msg += modeName[_modeCur];
  uint32_t id = DEVICE_ID_BRIDGE1;
  mesh.sendSingle(id, msg);
  if (DEBUG) { Serial.println(msg); }
  //shouldSaveSettings = true;
}


