/*  _____       _       
 * |  __ \     (_)      
 * | |__) |__ _ _ _ __  
 * |  _  // _` | | '_ \ 
 * | | \ \ (_| | | | | |
 * |_|  \_\__,_|_|_| |_|
 *
 * by Jan Dechent 
 * as part of https://github.com/jandechent/xyeShelf
 * using GPL-3.0 
 */

class xye_rain : public xye_animation {
  public:
    String get_animation_name() {return "Regen";};

    void rain_loop() {
      EVERY_N_MILLISECONDS(500){
        fadeToBlackBy(leds, kMatrixWidth*kMatrixHeight*kMatrixElements, 128);
        // clear bottom row
        for ( uint8_t x = 0; x < kMatrixWidth; x++) {
          setXY(x,0,CRGB::Black);
        }
        for ( uint8_t d = 0; d < nrain_drops; d++) {
          drops[d].fall();
          drops[d].draw();  
        }
        FastLED.show();
      }
    }

  private:
    const uint8_t nrain_drops = 3;

    class Drop{
      public:
        Drop();
        void fall();
        void draw();
        int8_t x;
        int8_t y;
        int8_t vel;
        int8_t len;
    };

    Drop drops [nrain_drops];

    Drop::Drop(){
      this->vel=1;
      this->x =random(  kMatrixWidth );
      this->y = kMatrixHeight-1-random(3);
    }

    void Drop::fall(){
      this->y-=vel;

      if(this->y<0){
        this->y = kMatrixHeight-1;
        this->x = random(  kMatrixWidth );
      }
    }

    void Drop::draw(){
      if(this->y==0){
        setXY(this->x,this->y,CRGB(0,0,255));
      }else{
        setXY(this->x,this->y,CRGB(0,127,255));
      }
    }

    //
    ////Digital Rain implementation
    //// inspired by Yaroslaw Turbin 24.08.2020
    //// https://vk.com/ldirko
    //// https://www.reddit.com/user/ldirko/
    //
    //
    //uint8_t rain[kMatrixWidth*kMatrixHeight];
    //
    //uint8_t rain_speed = 1;
    //int8_t rain_inc = 1;
    //
    //const float rain_target_fraction_of_initial_pixels = 0.02;
    //const uint8_t rain_update_every_n_millis= 500;
    //const uint8_t rain_changepattern_every_n_millis= 80;
    //
    //
    //void rain_loop() {
    //  EVERY_N_MILLISECONDS(rain_update_every_n_millis) {
    //    rain_update();
    //    FastLED.show();
    //  }
    //  EVERY_N_MILLISECONDS(rain_update_every_n_millis/2) {
    //   fadeToBlackBy(leds, kMatrixWidth*kMatrixHeight*kMatrixElements, 50);
    //   //blurColumns(leds, kMatrixWidth*kMatrixElements, kMatrixHeight, 8);
    //    FastLED.show();
    //  }
    //  /*EVERY_N_MILLISECONDS(1000){
    //    rain_change_by(rain_inc);
    //  }
    //  EVERY_N_MILLISECONDS(5000){
    //    rain_inc=-rain_inc;
    //  }*/
    //  EVERY_N_MILLISECONDS(rain_changepattern_every_n_millis) {
    //    rain_changepattern();
    //  }
    //} //loop
    //
    //void rain_changepattern () {
    //  uint8_t rand1 = random16 (kMatrixWidth*kMatrixHeight);
    //  uint8_t rand2 = random16 (kMatrixWidth*kMatrixHeight);
    //  if ((rain[rand1] == 1) && (rain[rand2] == 0) )   //simple get two random dot 1 and 0 and swap it,
    //  {
    //    rain[rand1] = 0;  //this will not change total number of dots
    //    rain[rand2] = 1;
    //  }
    //}
    //
    //void rain_init() {
    //  rain_speed = 1;
    //  rain_inc = 1;
    //  FastLED.clear();
    //  
    //  Serial.println("Initizaliting Rain:");
    //  Serial.print(" - Pixels, target prob [%]: ");  Serial.println( 100*rain_target_fraction_of_initial_pixels);
    //  uint8_t r8_input = 1.0/rain_target_fraction_of_initial_pixels;
    //  uint8_t amount_of_rain=0;
    //  for (uint8_t i = 0; i < (kMatrixWidth*kMatrixHeight); i++) {
    //    if (random8(r8_input) == 0) {
    //      rain[i] = 1;
    //      amount_of_rain++;
    //    } else {
    //      rain[i] = 0;
    //    }
    //  }
    //  if (amount_of_rain==0){
    //    Serial.println(" - Forcing at least one pixel...");
    //    rain[0] = 1;
    //    amount_of_rain++;
    //  }
    //  Serial.print(" - Pixels, actual      [#]: "); Serial.println( amount_of_rain );
    //  float changepattern_rate = rain_target_fraction_of_initial_pixels*(1-rain_target_fraction_of_initial_pixels) / rain_changepattern_every_n_millis * 1000;
    //  Serial.print(" - Change rate       [1/s]: "); Serial.println( changepattern_rate );
    //}
    //
    //void rain_update() {
    //  for (uint8_t x = 0; x < kMatrixWidth; x++) {
    //    for (uint8_t y = 0; y < kMatrixHeight; y++) {
    //      uint8_t y_dist=((y + rain_speed + random8(2) + kMatrixHeight) % kMatrixHeight);//random8(1) +
    //      uint8_t layer = rain[XY0(x,y_dist)];
    //      if (layer) { setXY(x,  y, CRGB(0, 0, 255)); }
    //    }
    //  }
    //  rain_speed++;
    //}
    //
    //void rain_change_by(int8_t delta){
    //  for (uint8_t i = 0; i < (kMatrixWidth*kMatrixHeight); i++) {
    //    if(delta > 0 && rain[i]==0){
    //      rain[i]=1;
    //      return;
    //    }
    //    if(delta < 0 && rain[i]==1){
    //      rain[i]=0;
    //      return;
    //    }
    //  }
    //}
};
