/*----------------------------time----------------------------*/

void setupTime() {
  //RTC.formatEEPROM();

  setSyncProvider(RTC.get);                   // The function to get the time from the RTC
  
  //RTC.promptForTimeAndDate(Serial);
  //Serial.println(F("Current date/time = "));
  //RTC.printTo(Serial);
  
  //setSunRise(9, 30);      //TEMP
  
  if (DEBUG_TIME) {
    if(timeStatus()!= timeSet) { Serial.println("Unable to sync with the RTC"); }
    else { Serial.println("RTC has set the system time"); }
  }
}
