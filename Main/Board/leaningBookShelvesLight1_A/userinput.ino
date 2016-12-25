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
long _touchSensorThreshold = 100;  //??? no idea - start at 100 and test  //unsigned long   //1 for all at the moment

//1,000 microseconds in a millisecond and 1,000,000 microseconds in a second
//eg. 6000 millisecond = 0.6 milliseconds = 0.0006 seconds
//micros() may be too small, might end up using millis()
//const int _touchDeBounceInterval = 10000;                            //interval to de-bounce in microseconds
const long _touchDeBounceInterval = 500;                            //interval to de-bounce in milliseconds    //const int 
//long _touch0prevMillis = 0;                                         //how long between 'bounces' //unsigned long
long _touchPrevMillis[4] = { 0, 0, 0, 0 };
boolean _touchToggled[4] = { false, false, false, false };

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
    
  if(_touchToggled[0] == true) {
    #ifdef DEBUG
      //Serial.print(" ttoggled == true");
      //Serial.println();
    #endif
    long touch0curMillis = millis();            //get current micros() //unsigned long
    if((long) (touch0curMillis - _touchPrevMillis[0]) >= _touchDeBounceInterval) {  // if((unsigned long)..
      _touchToggled[0] = false;                   //reset
    }
  } //END _touchToggled[0] == true

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
    
  if(_touchToggled[1] == true) {
    #ifdef DEBUG
      //Serial.print(" ttoggled == true");
      //Serial.println();
    #endif
//      unsigned long touch0curMicros = micros();     //get current micros()
//      if((unsigned long) (touch0curMicros - _touch0prevMicros) >= _touchDeBounceInterval) {
//        _touchToggled[0] = false;                   //reset
//      }
      long touch1curMillis = millis();     //get current micros() //unsigned long
      if((long) (touch1curMillis - _touchPrevMillis[1]) >= _touchDeBounceInterval) {  // if((unsigned long)..
      _touchToggled[1] = false;                   //reset
    }
  } //END _touchToggled[1] == true

/*---------------touch sensor 2 - sub-mode--------------*/
/*
    if(touch1read > _touchSensorThreshold) {
    
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
        
    } //END touch1read > _touchSensorThreshold
*/

/*---------------touch sensors 3 and 4 - brightness--------------*/
//touch read 2 and 3 need to be grouped together to try for slide strip

} //END touchSensors

