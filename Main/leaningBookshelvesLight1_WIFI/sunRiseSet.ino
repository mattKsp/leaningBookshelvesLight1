/*----------------------------mode1 sunrise and mode6 sunset----------------------------*/

// sunrise and sunset modes (1 & 6) are usually kept out of the preset lists and just triggered by the alarm interrupts
// ..but it is a mode and can be selected as such 

// Sunrise
void mode1() {
  doSunRise();

  if (_sunRiseTriggered) {
    _modeCur = _saveCurMode;                  // Restore previous mode
    _sunRiseTriggered = false;
  }
}

// Sunset
void mode6() {
  doSunSet();

  if (_sunSetTriggered) {
    _modeCur = _saveCurMode;                  // Restore previous mode
    _sunSetTriggered = false;
  }

}

/*----------------------------do----------------------------*/
/*
 * sunrise/set states can be used by such as static/moving
 * eg. static could go straight to the end colours, or the frozen midpoint of the 'do' state
 * eventually one would be able to use scroll wheels or sim to move throughout the animation, stop, reverse, etc.
 * 
 * //setup
 * beginning
 * do rise
 * end
 */

void doSunRise() { }
 
void doSunSet() { }
