/*----------------------------user input----------------------------*/

void setupUserInputs() {
  //touch sensors already setup..
}

/*
 * user inputs loop
 * called from main loop
 */
void loopUserInputs() {
  touchSensors();
}

/*----------------------------touch sensors----------------------------*/

void touchSensors() {
  //mode
  //sub-mode
  //
  //brightness up
  //brightness down

  /*---------------touch sensor 0 - on/off--------------*/
  //_onOff = !_onOff;                         //flip the lights
     
  #ifdef DEBUG
    softSerial.print(F("touch 0 triggered"));
    softSerial.println();
    softSerial.print(F("on/off = "));
    softSerial.print(_onOff);
    softSerial.println();
  #endif
      
  /*---------------touch sensor 1 - mode--------------*/
  if(_onOff == false) {
    _onOff = true;  //if the lights are already off, then turn them on
    //don't need to change mode, as we are already in a mode, just switched on
  } else {
    incrementPresetSlot();
    //write cur mode to memory ???
  } //END onOff
  
  #ifdef DEBUG
    softSerial.print(F("touch 1 triggered"));
    softSerial.println();
    softSerial.print(F("Mode = "));
    softSerial.print(_modeCur);
    softSerial.println();
  #endif
      
  /*---------------touch sensor 2 - --------------*/

  /*---------------touch sensor 3 - brightness up--------------*/
  //increaseBrightness();
  //FastLED.setBrightness(_ledGlobalBrightnessCur);
     
  #ifdef DEBUG
    softSerial.print(F("touch 2 triggered"));
    softSerial.println();
    softSerial.print(F("brightness = "));
    softSerial.print(_ledGlobalBrightnessCur);
    softSerial.println();
  #endif

  /*---------------touch sensor 4 - brightness down--------------*/
  //decreaseBrightness();
  //FastLED.setBrightness(_ledGlobalBrightnessCur);
  
  #ifdef DEBUG
    softSerial.print(F("touch 3 triggered"));
    softSerial.println();
    softSerial.print(F("brightness = "));
    softSerial.print(_ledGlobalBrightnessCur);
    softSerial.println();
  #endif



} //END touchSensors

