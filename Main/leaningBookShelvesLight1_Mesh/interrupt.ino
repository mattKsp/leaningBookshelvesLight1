/*----------------------------interrupt----------------------------*/

/*
 * Sunrise and Sunset interrupts. 
 * The DS3231 only has one alarm output so sunrise/sunset will be differentianted by AM/PM.
 * Sunrise will use Alarm 1 and Sunset will use Alarm 2.
 */
void setupInterrupts() {
  pinMode(_i2cInterrupt1Pin, INPUT_PULLUP); // Setup DS3231 interrupt pin 1 - Sunrise/Sunset
  //attachInterrupt(interruptPin, function, CHANGE); //FALLING, RISING, CHANGE
  attachInterrupt(digitalPinToInterrupt(_i2cInterrupt1Pin), sunRiseSetInterrupt, FALLING);
}

/*
 * ..keep interrupts short, yes i know, but in this case there is nothing time sensitive, no other interrupts etc.,
 * in short, i am not concerned over the length of the following function
 */
void sunRiseSetInterrupt() {
  boolean amPm = false;
  if(_firstTimeSetupDone) {
    // This has to be wrapped in '_firstTimeSetupDone' because interrupt has a tendancy to trigger at the start.
   
    // Check if AM or PM
    tmElements_t timeStamp;                   // Create a variable to hold the data 
    timeStamp = GetTime();                    // Get the time
    if (timeStamp.Hour < 12) {
      amPm = false;
    } else {
      amPm = true;
    }
 
    if (amPm) {
    // If AM
      _sunRiseTriggered = true;               // Set false after sunrise
      _saveCurMode = _modeCur;                // Save current mode and restore after sunrise
      _modeCur == 1;
      _sunRiseStateCur = 0;
      _onOff = true;
    } else {
      // If PM

      _sunSetTriggered = true;                // Set false after sunset
      _saveCurMode = _modeCur;                // Save current mode and restore after sunset
      _modeCur == 6;
      _sunSetStateCur = 0;
      _onOff = true;
    }
  }
  
  if (DEBUG_GEN) { 
    Serial.print(F("Interrupt triggered"));
    Serial.println();
    if (amPm) {
      Serial.println(F("SunRise"));
    } else {
      Serial.println(F("SunSet"));
    }
  }
}
