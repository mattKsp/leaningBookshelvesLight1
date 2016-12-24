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
byte _touchSensorRes = 20;  //sample/sensor resolution - higher is better but slower to read
unsigned long _touchSensorThreshold = 1000;  //??? no idea - start at 100 and test

void touchSensors() {

  //long _touch0read = _touch0.capacitiveSensor(_touchSensorRes);
  long _touch0read = _touch0.capacitiveSensor(_touchSensorRes);     //mode
  //long _touch1read = _touch1.capacitiveSensor(_touchSensorRes);     //sub-mode
  long _touch2read = _touch2.capacitiveSensor(_touchSensorRes);     //brightness up
  long _touch3read = _touch3.capacitiveSensor(_touchSensorRes);     //brightness down
  
  Serial.print(_touch0read);
  Serial.println();
  
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

/*
    if(_touch1read > _touchSensorThreshold) {
    
        if(_onOff == false) {
          _onOff = true;  //if the lights are already off, then turn them on
          //don't need to change mode, as we are already in a mode, just switched on
        } else {
          //change sub-mode
          //write cur sub-mode to memory ???
        } //END onOff
        
        #ifdef DEBUG
          Serial.print(F("touch 1 triggered"));
          Serial.println();
          Serial.print(F("sub-mode = "));
          Serial.print();
          Serial.println();
        #endif
        
    } //END _touch1read > _touchSensorThreshold
*/

//touch read 2 and 3 need to be grouped together to try for slide strip

} //END touchSensors

