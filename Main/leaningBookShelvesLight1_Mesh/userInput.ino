/*----------------------------user input----------------------------*/

void setupUserInputs() {
  touch.begin();
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

