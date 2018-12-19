/*----------------------------user input----------------------------*/

void setupUserInputs() {
  touch.begin();
}

/*
 * user inputs loop
 * called from main loop
 */
void loopUserInputs() {
  touchSensorsSimple();
  //touchSensors();                             // CAP1296
}

/*----------------------------touch sensors----------------------------*/
void touchSensorsSimple() {
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
  
}

void touchSensors() {
  int newTouch = 0;                           //'0' is nothing, range is 1-6
  //int newRelease = 0;                         //'0' is nothing, range is 1-6
  //mode
  //sub-mode
  //
  //brightness up
  //brightness down

//mabye put in a delay of 50 for all this touch stuff.. ???
  if (touch.touchStatusChanged()) {
    touch.updateTouchData();
    //we are not using multi-touch, so there should only be 1 sensor triggered at a time..
    //do we want it on 'touch', or on 'release' ???
    //..touch. leave fancy holds for later..
    for (int i = 0; i < 6; i++) {
      if (touch.isNewTouch(i)) {
        newTouch = i;
        if (DEBUG) {
          Serial.println("Touch sensor " + String(i) + " was just touched.");
        }
      }
//      if (touch.isNewRelease(i)) {
//        newRelease = i;
//        if (DEBUG) {
//          Serial.println("Touch sensor " + String(i) + " was just released.");
//        }
//      }
    }
  }

  switch (newTouch) {

    case 1:
      /*---------------touch sensor 1 - on/off--------------*/
      _onOff = !_onOff;                         //flip the lights
      if (DEBUG) {
        Serial.print(F("touch 1 triggered"));
        Serial.println();
        Serial.print(F("on/off = "));
        Serial.print(_onOff);
        Serial.println();
      }
      break;

    case 2:
      /*---------------touch sensor 2 - mode--------------*/
      if(_onOff == false) {
        _onOff = true;  //if the lights are already off, then turn them on
        //don't need to change mode, as we are already in a mode, just switched on
      } else {
        incrementPresetSlot();
        //write cur mode to memory ???
      } //END onOff
      
      if (DEBUG) {
        Serial.print(F("touch 2 triggered"));
        Serial.println();
        Serial.print(F("Mode = "));
        Serial.print(_modeCur);
        Serial.println();
      }
      break;

    case 3:
      /*---------------touch sensor 3 - --------------*/
      
      if (DEBUG) {
        Serial.print(F("touch 3 triggered"));
        Serial.println();
        Serial.print(F(" = "));
        //Serial.print();
        Serial.println();
      }
      break;

    case 4:
      /*---------------touch sensor 4 - brightness up--------------*/
      increaseBrightness();
      //FastLED.setBrightness(_ledGlobalBrightnessCur);                 // ???
         
      if (DEBUG) {
        Serial.print(F("touch 4 triggered"));
        Serial.println();
        Serial.print(F("brightness = "));
        Serial.print(_ledGlobalBrightnessCur);
        Serial.println();
      }
      break;

    case 5:
      /*---------------touch sensor 5 - brightness down--------------*/
      decreaseBrightness();
      //FastLED.setBrightness(_ledGlobalBrightnessCur);                 // ???
      
      if (DEBUG) {
        Serial.print(F("touch 5 triggered"));
        Serial.println();
        Serial.print(F("brightness = "));
        Serial.print(_ledGlobalBrightnessCur);
        Serial.println();
      }
      break;

    case 6:
      /*---------------touch sensor 6--------------*/
      
      if (DEBUG) {
        Serial.print(F("touch 6 triggered"));
        Serial.println();
        Serial.print(F(" = "));
        //Serial.print();
        Serial.println();
      }
      break;

  } //END switch case


} //END touchSensors

