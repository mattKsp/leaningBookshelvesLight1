/*----------------------------HVAC - Inc. Control enclosure and Bookshelves--------------------------*/
//                                            //

/*
 * Cooling currently just uses a temperature sensor to turn fans on and off.
 * 
 * This may also include plant well-being and local area climate control contribution.
 */
 
void setupHvac() 
{
  // setup temperature sensors
  // setup fan speed sensors
}

byte _temperatureEnclosureSpeedPos = 0;  // Temporary temperature position (within a given range) from the control enclosure, used for calculating fan speed control.

void loopHvac()
{
  // get temperatures
  // check temperature ranges
  //_temperatureEnclosureCur
  //_temperatureShelvesCur[4][8]
  //_temperatureTopCur
  boolean _fansFloorEnabled = false;
  byte _fansFloorSpeed = 255;
  boolean _flapsFloorEnabled = false;
  byte _flapsFloorAngleCur = 0;  // expecting to use at most 0-180
  byte _flapsFloorAngleMin = 0;
  byte _flapsFloorAngleMax = 180; 
  
  // Turn On/Off and set fan speeds (based upon temperature mapped within given Off/On ranges (Min/Max)).
  // set Floor fans and Floor flaps.
  //_flapsFloorAngleCur = 0;
  //if (_flapsFloorAngleCur < _flapsFloorAngleMin) { _flapsFloorAngleCur = _flapsFloorAngleMin; _flapsFloorEnabled = false; }
  //if (_flapsFloorAngleCur > _flapsFloorAngleMin) { _flapsFloorEnabled = true; }
  //if (_flapsFloorAngleCur > _flapsFloorAngleMax) { _flapsFloorAngleCur = _flapsFloorAngleMax; }
  //if (_flapsFloorEnabled == true) {
  //  //..can now turn On floorFans
  //  _fansFloorSpeed = based upon something;
  //  _fansFloorEnabled = true;
  //}
  // set Enclosure fans.
  if (_temperatureEnclosureCur <= _temperatureEnclosureOff[1])   { _temperatureEnclosureSpeedPos = 0; _fansEnclosureEnabled = false; }
  if (_temperatureEnclosureCur > _temperatureEnclosureOff[1]) {
    _temperatureEnclosureSpeedPos = map( _temperatureEnclosureCur, _temperatureEnclosureOff[1], _temperatureEnclosureOn[1], 0, 255);
    _fansEnclosureEnabled = true; }
  if (_temperatureEnclosureCur >= _temperatureEnclosureOn[1]) { _temperatureEnclosureSpeedPos = 255; _fansEnclosureEnabled = true; }
  _fansEnclosureSpeed = _temperatureEnclosureSpeedPos;  
  // set Shelf fans.
  //..
  // set Top fan.
  if (_temperatureTopCur > 25) {
    //_fansTopSpeed
    _fansTopEnabled = true;
  } else {
    _fansTopEnabled = false;
  }
  
  
}
