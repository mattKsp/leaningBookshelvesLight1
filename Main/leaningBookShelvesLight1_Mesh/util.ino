/*----------------------------util----------------------------*/

/*
 * Mode utils
 */
void incrementPresetSlot() {
  _modePresetSlotCur += 1;
  incDecPresSlot_prtB();
}
void decrementPresetSlot() {
  _modePresetSlotCur -= 1;
  incDecPresSlot_prtB();
}
void incDecPresSlot_prtB() {
  if(_modePresetSlotCur >= _modePresetSlotNum){ _modePresetSlotCur = 0; }  //TEMP rollover catch
  _modeCur = _modePreset[_modePresetSlotCur];
}

/*
 * Golbal brightness utils
 */
void setGlobalBrightness(int gb) {
  //use this to achieve an override from the mesh, eg. to match levels
  _ledGlobalBrightnessCur = gb;
  brightnessRolloverCatch();
}
void increaseBrightness() {
  _ledGlobalBrightnessCur += _ledBrightnessIncDecAmount;
  brightnessRolloverCatch();
}
void decreaseBrightness() {
  _ledGlobalBrightnessCur -= _ledBrightnessIncDecAmount;
  brightnessRolloverCatch();
}
void brightnessRolloverCatch() {
  if(_ledGlobalBrightnessCur > 255) {
    _ledGlobalBrightnessCur = 255;
  } else if(_ledGlobalBrightnessCur < 0) {
    _ledGlobalBrightnessCur = 0;
  }
}

/*
 Check Segment Endpoints
 usage | checkSegmentEndpoints();
 overlay/place near end of chain
 This places different coloured lights at the ends of the LED strip(s) segments for quick visual feedback of calculations
 */
void checkSegmentEndpoints() {
  
  for(int i = 0; i < _ledNumOfStrips; i++) {
    fill_solid( leds[i], _ledNumPerStrip, CRGB(0,0,0));
    
    for(int j = 0; j < _segmentTotal; j++) {
      leds[i][ledSegment[_segmentTotal].first] = CRGB(i*20, j*50, 0);
      leds[i][ledSegment[_segmentTotal].last] = CRGB(0, j*50, i*20);
    }
  }
  
}

/*
 Show colour temperature indicator pixel
 usage | showColorTempPx();
 overlay/place near end of chain
 This sets the first LED to show the current colour temperature
 */
void showColorTempPx() {
  leds[0][0] = _colorTempCur;                  //show indicator pixel
  //leds[0] = TEMPERATURE_1;                  //show indicator pixel
}

/*
 Get Voltage
 usage | Serial.println( getVoltage(), DEC );
 This compares Vcc against internal 1.1v reference and returns the result in mV (eg. 5000mV = 5v)
 ATmega168 & 328 chips can use this trick to read their own voltage rails
 https://code.google.com/archive/p/tinkerit/wikis/SecretVoltmeter.wiki
 */
 /*
long getVoltage() { 
  long result;                              // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); 
  delay(2);                                 // Wait for Vref to settle 
  ADCSRA |= _BV(ADSC);                      // Convert 
  while (bit_is_set(ADCSRA,ADSC)); 
  result = ADCL; 
  result |= ADCH<<8; 
  result = 1126400L / result;               // Back-calculate AVcc in mV
  return result;
}
*/

/*
  SerialEvent occurs whenever a new data comes in the
  hardware serial RX.  This routine is run between each
  time loop() runs, so using delay inside loop can delay
  response.  Multiple bytes of data may be available.
*/
//void serialEvent() {
//  while (Serial.available()) {
//    // get the new byte:
//    char inChar = (char)Serial.read();
//    // add it to the inputString:
//    _inputString += inChar;
//    // if the incoming character is a newline, set a flag
//    // so the main loop can do something about it:
//    if (inChar == '\n') {
//      _stringComplete = true;
//    }
//  }
//}


/*
 * Serial promt to set alarm 2 - sunrise
 */
/*#ifdef SET_SUNRISE_ALARM_BY_SERIAL
void promptForSunRiseAlarm(Stream &Serial)
{
  char buffer[3] = { 0 };
    
  Serial.println();
  Serial.print(F("SunRise (Alarm2) is set when all data is entered and you send 'Y' to confirm."));
  do
  {
    memset(buffer, 0, sizeof(buffer));
    Serial.println();
    Serial.print(F("Enter Hour (2 digits, 24 hour clock, 00-23): "));
    while(!Serial.available()) ; // Wait until bytes
    Serial.readBytes(buffer, 2);
    while(Serial.available()) Serial.read(); 
    int hr = atoi(buffer[0] == '0' ? buffer+1 : buffer);
    
    memset(buffer, 0, sizeof(buffer));
    Serial.println();
    Serial.print(F("Enter Minute (2 digits, 00-59): "));
    while(!Serial.available()) ; // Wait until bytes
    Serial.readBytes(buffer, 2);
    while(Serial.available()) Serial.read(); 
    int mins = atoi(buffer[0] == '0' ? buffer+1 : buffer);

    Serial.println();
    Serial.print(F("Entered SunRise (Alarm2) @ ")); 
    //printTo(Serial, Settings);
    Serial.print(hr);
    Serial.print(":"); 
    Serial.print(mins); 
     
    Serial.println();
    Serial.print(F("Send 'Y' to set SunRise (Alarm2), send 'N' to start again: "));
        
    while(!Serial.available()) ; // Wait until bytes
    Serial.readBytes(buffer, 1);
    while(Serial.available()) Serial.read(); 
    if(buffer[0] == 'Y' || buffer[0] == 'y')
    {
      setSunRise(1, hr, mins);    //mode 1 = set time this way
      Serial.println();
      Serial.print(F("SunRise (Alarm2) set"));
      Serial.println();
      break;
    }
  } while(1);   
}
#endif */

/*
 * Serial promt to set Alarm 1 - sunset
 */
/*#ifdef SET_SUNSET_ALARM_BY_SERIAL
void promptForSunSetAlarm(Stream &Serial)
{
  char buffer[3] = { 0 };
    
  Serial.println();
  Serial.print("SunSet (Alarm1) is set when all data is entered and you send 'Y' to confirm.");
  do
  {
    memset(buffer, 0, sizeof(buffer));
    Serial.println();
    Serial.print("Enter Hour (2 digits, 24 hour clock, 00-23): ");
    while(!Serial.available()) ; // Wait until bytes
    Serial.readBytes(buffer, 2);
    while(Serial.available()) Serial.read(); 
    int hr = atoi(buffer[0] == '0' ? buffer+1 : buffer);
    
    memset(buffer, 0, sizeof(buffer));
    Serial.println();
    Serial.print("Enter Minute (2 digits, 00-59): ");
    while(!Serial.available()) ; // Wait until bytes
    Serial.readBytes(buffer, 2);
    while(Serial.available()) Serial.read(); 
    int mins = atoi(buffer[0] == '0' ? buffer+1 : buffer);

    Serial.println();
    Serial.print("Entered SunSet (Alarm1) - "); 
    Serial.print(hr);
    Serial.print(":"); 
    Serial.print(mins); 
     
    Serial.println();
    Serial.print("Send 'Y' to set Sunset (Alarm1), send 'N' to start again: ");
        
    while(!Serial.available()) ; // Wait until bytes
    Serial.readBytes(buffer, 1);
    while(Serial.available()) Serial.read(); 
    if(buffer[0] == 'Y' || buffer[0] == 'y')
    {
      setSunSet(0, hr, mins);    //mode 1 = set time this way
      Serial.println();
      Serial.print("SunSet (Alarm1) set"); 
      break;
    }
  } while(1);   
}
#endif */

