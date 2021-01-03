/*----------------------------cooling--------------------------*/
//                                            //

/*
 * Cooling currently just uses a temperature sensor to turn fans on and off.
 */
 
void setupCooling() 
{
  // setup temperature sensor
}

void loopCooling()
{
  // get temperature
  //_temperatureCur
  // check temperature range (implement later)
  if (      _temperatureCur >= _temperatureOn ) { _fansEnabled = true;  }
  else if ( _temperatureCur <= _temperatureOff) { _fansEnabled = false; }
}
