/*----------------------------user input----------------------------*/

void setupUserInputs() {
  
  //button0
  pinMode(_button0Pin, INPUT_PULLUP);       //Setup the button with an internal pull-up :
  _button0.attach(_button0Pin);             //After setting up the button, setup the Bounce instance :
  _button0.interval(_buttonDebounceTime);
  
  //touch sensors already setup..
}

/*
 * user inputs loop
 * called from main loop
 */
void loopUserInputs() {
  button0();
  touchSensors();
}

/*----------------------------buttons----------------------------*/
void button0() {
  boolean hasChanged0 = _button0.update();  //Bounce buttons keep internal track of whether any change has occured since last time
  
  if(hasChanged0) {
    //_button[0].update();                  //Update the LED button Bounce instance again  ???
      //do button stuff
      if(_button0.fell()) {
        
        if(_onOff == false) {
          _onOff = true;  //if the lights are already off, then turn them on
          //don't need to change mode, as we are already in a mode, just switched on
        } else {
          incrementPresetSlot();
//          _modePresetSlotCur +=1;
//          if(_modePresetSlotCur >= _modePresetSlotNum){ _modePresetSlotCur = 0; }  //TEMP rollover catch
//          _modeCur = _modePreset[_modePresetSlotCur];
          //write cur mode to memory ???
        } //END onOff
        
        #ifdef DEBUG
          Serial.print(F("button 0 - fell"));
          Serial.println();
          Serial.print(F("Mode = "));
          Serial.print(_modeCur);
          Serial.println();
        #endif
        
      } //END fell
      
      if(_button0.rose()) {
        //write cur mode to memory ???
      } //END rose
    
  } //END hasChanged0
  
} //END button0

/*----------------------------touch sensors----------------------------*/
byte _touchSensorRes = 30;  //sample/sensor resolution
unsigned long _touchSensorThreshold = 100;  //??? no idea - start here and test

void touchSensors() {

  //long _touch0read = _touch0.capacitiveSensor(_touchSensorRes);
  long _touch0read = _touch0.capacitiveSensor(_touchSensorRes);
  long _touch1read = _touch1.capacitiveSensor(_touchSensorRes);
  long _touch2read = _touch2.capacitiveSensor(_touchSensorRes);
  long _touch3read = _touch3.capacitiveSensor(_touchSensorRes);
  
  if(_touch0read > _touchSensorThreshold) {
    
    if(_onOff == false) {
          _onOff = true;  //if the lights are already off, then turn them on
          //don't need to change mode, as we are already in a mode, just switched on
        } else {
          incrementPresetSlot();
          //write cur mode to memory ???
        } //END onOff
        
        #ifdef DEBUG
          Serial.print(F("touch 0 triggered"));
          Serial.println();
          Serial.print(F("Mode = "));
          Serial.print(_modeCur);
          Serial.println();
        #endif
        
    } //END _touch0read > _touchSensorThreshold

} //END touchSensors


//for (i = 0; i < (sizeof(myInts)/sizeof(int)) - 1; i++) {
//  // do something with myInts[i]
//}

