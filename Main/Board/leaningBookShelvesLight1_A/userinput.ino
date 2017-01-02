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
  //long _touch0read = _touch0.capacitiveSensor(_touchSensorRes);     //mode
  //long touch1read = _touch1.capacitiveSensor(_touchSensorRes);     //sub-mode
  //long _touch2read = _touch2.capacitiveSensor(_touchSensorRes);     //brightness up
  //long _touch3read = _touch3.capacitiveSensor(_touchSensorRes);     //brightness down

/*---------------touch sensor 0 - on/off--------------*/
  if(_touchToggled[0] == false) {
    long touch0read = _touch0.capacitiveSensor(_touchSensorRes);     //mode
    #ifdef DEBUG
      //Serial.print(touch0read);
      //Serial.print(" 0toggled == false");
      //Serial.println();
    #endif
    if(touch0read > _touchSensorThreshold) {
      _touchToggled[0] = true;                      //toggle so we can block re-bounce
      _touchPrevMillis[0] = millis();               //store the current time
      
      _onOff = !_onOff;                         //flip the lights
         
      #ifdef DEBUG
        Serial.print(F("0touch 0 triggered"));
        Serial.println();
        Serial.print(F("on/off = "));
        Serial.print(_onOff);
        Serial.println();
      #endif
      
    } //END _touch0read > _touchSensorThreshold
  } //END _touchToggled[0] == false
    
/*---------------touch sensor 1 - mode--------------*/
  if(_touchToggled[1] == false) {
    long touch1read = _touch1.capacitiveSensor(_touchSensorRes);     //mode
    #ifdef DEBUG
      //Serial.print(touch1read);
      //Serial.print(" 1toggled == false");
      //Serial.println();
    #endif
    if(touch1read > _touchSensorThreshold) {
      _touchToggled[1] = true;                    //toggle so we can block re-bounce
      //_touch0prevMicros = micros();               //store the current time
      _touchPrevMillis[1] = millis();               //store the current time
      
      if(_onOff == false) {
        _onOff = true;  //if the lights are already off, then turn them on
        //don't need to change mode, as we are already in a mode, just switched on
      } else {
        incrementPresetSlot();
        //write cur mode to memory ???
      } //END onOff
      
      #ifdef DEBUG
        Serial.print(F("1touch 1 triggered"));
        Serial.println();
        Serial.print(F("Mode = "));
        Serial.print(_modeCur);
        Serial.println();
      #endif
    } //END _touch1read > _touchSensorThreshold
  } //END _touchToggled[1] == false

/*---------------touch sensor 3 - brightness up--------------*/
//  if(_touchToggled[2] == false) {
//    long touch2read = _touch2.capacitiveSensor(_touchSensorRes);     //mode
//    #ifdef DEBUG
//      //Serial.print(touch0read);
//      //Serial.print(" 2toggled == false");
//      //Serial.println();
//    #endif
//    if(touch2read > _touchSensorThreshold) {
//      _touchToggled[2] = true;                      //toggle so we can block re-bounce
//      _touchPrevMillis[2] = millis();               //store the current time
//
//      increaseBrightness();
//      FastLED.setBrightness(_ledGlobalBrightnessCur);
//         
//      #ifdef DEBUG
//        Serial.print(F("2touch 2 triggered"));
//        Serial.println();
//        Serial.print(F("brightness = "));
//        Serial.print(_ledGlobalBrightnessCur);
//        Serial.println();
//      #endif
//      
//    } //END _touch2read > _touchSensorThreshold
//  } //END _touchToggled[2] == false
//  
/*---------------touch sensor 4 - brightness down--------------*/
//  if(_touchToggled[3] == false) {
//    long touch3read = _touch3.capacitiveSensor(_touchSensorRes);     //mode
//    #ifdef DEBUG
//      //Serial.print(touch3read);
//      //Serial.print(" 3toggled == false");
//      //Serial.println();
//    #endif
//    if(touch3read > _touchSensorThreshold) {
//      _touchToggled[3] = true;                      //toggle so we can block re-bounce
//      _touchPrevMillis[3] = millis();               //store the current time
//      
//      decreaseBrightness();
//      FastLED.setBrightness(_ledGlobalBrightnessCur);
//      
//      #ifdef DEBUG
//        Serial.print(F("3touch 3 triggered"));
//        Serial.println();
//        Serial.print(F("brightness = "));
//        Serial.print(_ledGlobalBrightnessCur);
//        Serial.println();
//      #endif
//      
//    } //END _touch3read > _touchSensorThreshold
//  } //END _touchToggled[3] == false
//   
 
  for(int i = 0; i < 5; i++) {
    if(_touchToggled[i] == true) {
      long touchcurMillis = millis();            //get current micros() //unsigned long
      if((long) (touchcurMillis - _touchPrevMillis[i]) >= _touchDeBounceInterval) {  // if((unsigned long)..
        _touchToggled[i] = false;                 //reset
      }
    }
  }

} //END touchSensors

