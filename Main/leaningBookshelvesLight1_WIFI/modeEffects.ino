/*----------------------------mode - effects----------------------------*/

/*--------Fire2012 by Mark Kriegsman, July 2012--------*/
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120


void fire2012()
{
  // Array of temperature readings at each simulation cell
  //static byte heat[_ledNumPerStrip-1];
  byte heat[_topLed];

  // Step 1.  Cool down every cell a little
  //for( int i = _ledSegment[1].first; i < (_ledNumPerStrip-1); i++) {
  for( int i = _ledSegment[1].first; i < (_topLed); i++) {
    //heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / (_ledNumPerStrip-1)) + 2));
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / (_topLed)) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  //for( int k = (_ledNumPerStrip - 1); k >= 2; k--) {
  for( int k = (_topLed); k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }
  
  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160,255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for(int i = 0; i < _ledNumOfStrips; i++) { 
    //for( int j = _ledSegment[1].first; j < (_ledNumPerStrip-1); j++) {
    for( int j = _ledSegment[1].first; j < (_topLed); j++) {
      CRGB color = HeatColor( heat[j]);
      //int pixelnumber;
      //bool gReverseDirection = false;
      //if( gReverseDirection ) {
      //  pixelnumber = (_ledNumPerStrip-1) - j;
      //} else {
      //  pixelnumber = j;
      //}
      //_leds[pixelnumber] = color;
      _leds[i][j] = color;
    }
    fill_gradient_RGB(_leds[i], _topLed+1, CRGB::Black, _ledSegment[_segmentTotal-1].last+1, CRGB::Black );
    _leds[i][0] = CRGB::Black;                                     // hack
  }
}

/*--------FastLED "100-lines-of-code" demo reel--------*/
// showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

/*--------"100-lines-of-code" - confetti--------*/
void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  for (int i = 0; i < _ledNumOfStrips; i++) {
    fadeToBlackBy(_leds[i], _ledNumPerStrip, 10);
    //int pos = random16(_ledNumPerStrip);
    int pos = random16(_topLed+1);
    _leds[i][pos] += CHSV( _gHue + random8(64), 200, 255);
    _leds[i][0] = CRGB::Black;                                     // hack
  }
}

/*--------"100-lines-of-code" - addGlitter(80)--------*/
// this doesn't work as is overlaying !!! - nice tho...  -- sorted now !!!
void glitter(fract8 chanceOfGlitter) 
{
  for (int i = 0; i < _ledNumOfStrips; i++) {
    // make everything black
    fadeToBlackBy( _leds[i], _ledNumPerStrip, 30); 
    // then add glitter
    if( random8() < chanceOfGlitter) {
      //_leds[i][ random16(_ledNumPerStrip) ] += CRGB::White;
      _leds[i][ random16(_topLed+1) ] += CRGB::White;
    }
    fill_gradient_RGB(_leds[i], _topLed+1, CRGB::Black, _ledSegment[_segmentTotal-1].last+1, CRGB::Black );
    _leds[i][0] = CRGB::Black;                                     // hack
  }
}
void addGlitter(fract8 chanceOfGlitter) 
{
  for (int i = 0; i < _ledNumOfStrips; i++) {
    if( random8() < chanceOfGlitter) {
      //_leds[i][ random16(_ledNumPerStrip) ] += CRGB::White;
      _leds[i][ random16(_topLed+1) ] += CRGB::White;
    }
    fill_gradient_RGB(_leds[i], _topLed+1, CRGB::Black, _ledSegment[_segmentTotal-1].last+1, CRGB::Black );
    _leds[i][0] = CRGB::Black;                                     // hack
  }
}

/*--------"100-lines-of-code" - rainbow--------*/
void rainbow() 
{
  // FastLED's built-in rainbow generator
  for (int i = 0; i < _ledNumOfStrips; i++) {
    fadeToBlackBy( _leds[i], _ledNumPerStrip, 30);
    //fill_rainbow( _leds[i], _ledNumPerStrip, _gHue, 7);
    fill_rainbow( _leds[i], _topLed_1, _gHue, 7);
    _leds[i][0] = CRGB::Black;                                     // hack
  }
}

/*--------"100-lines-of-code" - rainbowWithGlitter--------*/
void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

/*--------rain--------*/
// falling from top
void rain()
{
  //
}

/*--------solid--------*/
void solid()
{
  for (int i = 0; i < _ledNumOfStrips; i++) {
    //fill_solid( _leds[i], _ledNumPerStrip, effectColor_RGB);
    fadeToBlackBy( _leds[i], _ledNumPerStrip, 30);
    fill_gradient_RGB(_leds[i], _ledSegment[1].first, effectColor_RGB, _topLed, effectColor_RGB);
    //_leds[i][0] = CRGB::Black;                                     // hack
  }
}

/*--------_--------*/
/*--------_--------*/
/*--------_--------*/
/*--------_--------*/
/*--------_--------*/
