/*----------------------------interrupt----------------------------*/
//attachInterrupt(interruptPin, function, CHANGE);

void setupInterrupts() {
  pinMode(_i2cInterrupt1Pin, INPUT_PULLUP); //setup DS3231 interrupt pin
  attachInterrupt(digitalPinToInterrupt(_i2cInterrupt1Pin), sunRiseSetInterrupt, FALLING);  //FALLING, RISING, CHANGE
}

void sunRiseSetInterrupt() {
  //_sunRiseSetTriggered = true;
  //..keep interrupts short, yes i know, but in this case there is nothing time sensitive, no other interrupts etc.,
  //in short, i am not concerned over the length of the following function
  //sunRiseSetCheck();
  if(_firstTimeSetupDone) {
    _modeCur == 1;
    _sunRiseStateCur = 0;
    _onOff = true;
  }
  #ifdef DEBUG
    softSerial.print(F("interrupt triggered"));
    softSerial.println();
    statusLED.Blink1();
  #endif
}
