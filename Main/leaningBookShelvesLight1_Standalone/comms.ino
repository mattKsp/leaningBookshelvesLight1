/*----------------------------comms----------------------------*/

void comms() {
  receiveSerial();
  if (_newSerialData == true) {
    strcpy(_tempSerialChars, _receivedSerialChars);
        // this temporary copy is necessary to protect the original data
        //   because strtok() used in parseData() replaces the commas with \0
    parseIncomingSerialData();
    //showParsedData();
    _newSerialData = false;
  }
  #ifdef DEBUG
//    if (softSerial.available()) {
//      Serial.write(softSerial.read());
//    }
//    if (Serial.available()) {
//      softSerial.write(Serial.read());
//    }
  #endif
}

/*
 * receive serial comms with start and end markers
 * eg.  <A,0>
 *      <B,7,30>    //set sunRise at 7:30am
 *      ..note we would also have to enable sunRise as well!
 *      <D,1>
 */
void receiveSerial() {
  //previously 'recvWithStartEndMarkers'
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && _newSerialData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                _receivedSerialChars[ndx] = rc;
                ndx++;
                if (ndx >= _numSerialChars) {
                    ndx = _numSerialChars - 1;
                }
            }
            else {
                _receivedSerialChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                _newSerialData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void parseIncomingSerialData() {      
  //split the incoming serial data into its parts - previously 'parseData'

  char * strtokIndx; // this is used by strtok() as an index

  strtokIndx = strtok(_tempSerialChars,",");      // get the first part - the string
  strcpy(_messageFromSerial, strtokIndx); // copy it to messageFromPC
  
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  _integer1FromSerial = atoi(strtokIndx);     // convert this part to an integer
  
  if (strtokIndx == "A") {
    //_debugOverlay
    _debugOverlay = checkBool(_integer1FromSerial);
    #ifdef DEBUG
      softSerial.print(F("Debug Overlay = "));
      softSerial.println(_debugOverlay);
    #endif
  } 
  else if (strtokIndx == "B") {
    //setSunRise
    strtokIndx = strtok(NULL, ",");
    _integer2FromSerial = atoi(strtokIndx);   //convert to integer
    setSunRise(_integer1FromSerial, _integer2FromSerial);             //???
    _sunRiseEnabled = true;
    #ifdef DEBUG
      softSerial.print(F("Set SunRise for "));
      softSerial.print(_integer1FromSerial);
      softSerial.print(F(":"));
      softSerial.println(_integer2FromSerial);
    #endif
  } 
  else if (strtokIndx == "C") {
    //setSunSet
    strtokIndx = strtok(NULL, ",");
    _integer2FromSerial = atoi(strtokIndx);     //convert to integer
    //setSunSet(_integer1FromSerial, _integer2FromSerial);             //???
    _sunRiseEnabled = false;
    #ifdef DEBUG
      softSerial.print(F("Set SunSet for "));
      softSerial.print(_integer1FromSerial);
      softSerial.print(F(":"));
      softSerial.println(_integer2FromSerial);
    #endif
  } 
  else if (strtokIndx == "D") {
    //_sunRiseEnabled
    _sunRiseEnabled = checkBool(_integer1FromSerial);
    #ifdef DEBUG
      softSerial.print(F("SunRise Enabled = "));
      softSerial.println(_sunRiseEnabled);
    #endif
  } 
  else if (strtokIndx == "E") {
    //_sunSetEnabled
    _sunSetEnabled = checkBool(_integer1FromSerial);
    #ifdef DEBUG
      softSerial.print(F("SunSet Enabled = "));
      softSerial.println(_sunSetEnabled);
    #endif
  } 
  else if (strtokIndx == "F") {
    //setGlobalBrightness
    setGlobalBrightness(_integer1FromSerial);
    #ifdef DEBUG
      softSerial.print(F("Global Brightness ="));
      softSerial.println(_ledGlobalBrightnessCur);
    #endif
  } 
  else if (strtokIndx == "G") {
    //_modeCur
    _modeCur = _integer1FromSerial;
    #ifdef DEBUG
      softSerial.print(F("Current Mode = "));
      softSerial.println(_modeCur);
    #endif
  } 
  else if (strtokIndx == "H") {
    //_onOff
    _onOff = checkBool(_integer1FromSerial);
    #ifdef DEBUG
      softSerial.print(F("OnOff = "));
      softSerial.println(_onOff);
    #endif
  }

} //END parseIncomingSerialData

boolean checkBool(int i) {
  if (i == 1) {
    return true;
  } else {
    return false;
  }
}


