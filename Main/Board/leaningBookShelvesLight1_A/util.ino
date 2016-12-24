/*----------------------------util----------------------------*/

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

