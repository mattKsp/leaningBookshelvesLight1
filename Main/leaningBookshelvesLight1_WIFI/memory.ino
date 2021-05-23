/*----------------------------memory----------------------------*/

void loadSettings() {
  
}

void loopSaveSettings() {
  EVERY_N_SECONDS(60) {                       // too much ???
    if (_shouldSaveSettings == true)
    { 
      //saveSettings(); 
      _shouldSaveSettings = false; 
    }
  }
}
