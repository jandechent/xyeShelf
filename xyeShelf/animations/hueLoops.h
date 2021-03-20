//   _    _                             _                 _   _
//  | |  | |                /\         (_)               | | (_)
//  | |__| |_   _  ___     /  \   _ __  _ _ __ ___   __ _| |_ _  ___  _ __  ___
//  |  __  | | | |/ _ \   / /\ \ | '_ \| | '_ ` _ \ / _` | __| |/ _ \| '_ \/ __|
//  | |  | | |_| |  __/  / ____ \| | | | | | | | | | (_| | |_| | (_) | | | \__ \
//  |_|  |_|\__,_|\___| /_/    \_\_| |_|_|_| |_| |_|\__,_|\__|_|\___/|_| |_|___/

/*
int hueLoops_delay = 5;
uint8_t hueLoops_Hue = 0;
uint8_t hueLoops_Delta = 1;

void hueLoop_loopAll() {
  hueLoops_delay = 100;

  signalWhite();
  hueLoop_all();
  hueLoop_perLine();
  hueLoop_perRow();
  hueLoop_perPocket();

  hueLoops_delay = 10;
  hueLoop_perElement();
  hueLoop_advance();
}

void hueLoop_perElement() {
  for ( uint8_t y = 0; y < kMatrixHeight; y++) {
    for ( uint8_t x = 0; x < kMatrixWidth; x++) {
      for ( uint8_t e = 0; e < kMatrixElements; e++) {
        leds[XYE(x, y, e)].setHue(hueLoops_Hue);
        FastLED.show();
        delay(hueLoops_delay / kMatrixElements);
      }
      setXY(x, y, CRGB::Black);
    }
  }
  hueLoops_Hue += hueLoops_Delta;
}

void hueLoop_perPocket() {
  for ( uint8_t y = 0; y < kMatrixHeight; y++) {
    for ( uint8_t x = 0; x < kMatrixWidth; x++) {
      for ( uint8_t e = 0; e < kMatrixElements; e++) {
        leds[XYE(x, y, e)].setHue(hueLoops_Hue);
      }
      FastLED.show();
      delay(hueLoops_delay);
      setXY(x, y, CRGB::Black);
    }
  }
}

void hueLoop_perLine() {
  for ( uint8_t y = 0; y < kMatrixHeight; y++) {
    for ( uint8_t x = 0; x < kMatrixWidth; x++) {
      setXY(x, y, CHSV( hueLoops_Hue, 255, 255));
    }
    FastLED.show();
    for ( uint8_t x = 0; x < kMatrixWidth; x++) {
      setXY(x, y,    CRGB::Black);
    }
    delay(hueLoops_delay * kMatrixHeight / 2);
  }
}

void hueLoop_perRow() {
  for ( uint8_t x = 0; x < kMatrixWidth; x++) {
    for ( uint8_t y = 0; y < kMatrixHeight; y++) {
      setXY(x, y, CHSV( hueLoops_Hue, 255, 255));
    }
    FastLED.show();
    for ( uint8_t y = 0; y < kMatrixHeight; y++) {
      setXY(x, y,    CRGB::Black);
    }
    delay(hueLoops_delay * kMatrixWidth / 2);
  }
}

void hueLoop_all() {
  fill_solid( &(leds[0]), NUM_LEDS, CHSV( hueLoops_Hue, 255, 255) );
  FastLED.show();
  delay(hueLoops_delay * kMatrixElements * 2);
  //fill_solid( &(leds[0]), NUM_LEDS, CRGB::Black );
      hueLoops_Hue += hueLoops_Delta;
}

void hueLoop_advance() {
  hueLoops_Hue += hueLoops_Delta;
}

void signalWhite() {
  fill_solid( &(leds[0]), NUM_LEDS, CRGB::White );
  FastLED.show();
  delay(1000);
  fill_solid( &(leds[0]), NUM_LEDS, CRGB::Black );
  FastLED.show();
  delay(1000);
}*/
