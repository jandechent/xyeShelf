/** ______ _                           _                 _   _
 * |  ____(_)              /\         (_)               | | (_)
 * | |__   _ _ __ ___     /  \   _ __  _ _ __ ___   __ _| |_ _  ___  _ __
 * |  __| | | '__/ _ \   / /\ \ | '_ \| | '_ ` _ \ / _` | __| |/ _ \| '_ \
 * | |    | | | |  __/  / ____ \| | | | | | | | | | (_| | |_| | (_) | | | |
 * |_|    |_|_|  \___| /_/    \_\_| |_|_|_| |_| |_|\__,_|\__|_|\___/|_| |_|
 *
 * by Jan Dechent 
 * as part of https://github.com/jandechent/xyeShelf
 * using GPL-3.0 License 
 * 
 * Originally inspired by https://github.com/mic159/NeoFire/blob/master/NeoFire.ino
 * and adapted to to xyeShelf, including sparkels. 
 */

class xye_fire : public xye_animation {
  public:
    String get_animation_name() {return "Feuer";};
    String get_parameter_desciption() { 
      return this->get_animation_name() + "@" + String(int(100*pcnt_increment)) +"+‰";
    }
    byte get_parameter_value() {return param;};
    bool set_parameter(byte remote_param) {
      param = remote_param;
      pcnt_increment = (float) 1.5 * param/256;
      return true;
    };
    
    void setup() {
      pcnt = 0;
      FastLED.clear();
      randomSeed(analogRead(0));
      generateLine();
      memset(matrixValue, 0, sizeof(matrixValue));//init all pixels of the helper to zero
    }

    void loop() {
      if (pcnt >= 100) {
        shiftUp();
        generateLine();
        pcnt = 0;
      }
      drawFrame(pcnt);
      fire_sparkle();
      FastLED.show();
      pcnt += pcnt_increment;
    }

  private:
    byte param=95;
    float pcnt_increment = 1;    
    // these values are substracetd from the generated values to give a shape to the animation
    /*const unsigned char valueMask[kMatrixHeight][kMatrixWidth] = {
      {32 , 0  , 0  ,  0  , 32 },
      {64 , 32 , 0  ,  32 , 64 },
      {96 , 64 , 32 ,  64 , 96 },
      {128, 96 , 64 ,  96 , 128},
      {160, 128, 96 ,  128, 160},
      {192, 160, 128,  160, 192}
    };*/
    const unsigned char valueMask[kMatrixHeight][kMatrixWidth] = {
      {0  , 0  , 0  ,  0  , 0 },
      {16 , 16 , 16 ,  16 , 16 },
      {64 , 32 , 16 ,  32 , 64 },
      {64,  64 , 32 ,  64 , 64},
      {128, 128, 64 ,  128, 160},
      {160, 160, 128,  160, 160}
    };

    //these are the hues for the fire should be between 0 (red) to about 25 (yellow)
    const unsigned char hueMask[kMatrixHeight][kMatrixWidth] = {
      { 8 , 18, 25, 21,  8},
      { 7 , 15, 20, 15,  5},
      { 5 , 13, 16, 15,  5},
      { 5 , 11, 11, 11,  5},
      { 1 , 5 , 8 , 8 ,  1},
      { 0 , 1 , 5 , 5 ,  0},
    };

    const int fire_sparkle_time_ms = 1000; // 
    const uint8_t fire_sparkle_prob_over_per_time = 8; // so, turn up to turn sparks down.

    const float fire_sparkle_freq = (1000.0/(fire_sparkle_time_ms*fire_sparkle_prob_over_per_time));
                             
    float pcnt = 0;

    // Randomly generate the next line (matrix row)
    void generateLine() {
      for (uint8_t x = 0; x < kMatrixWidth; x++) {
        line[x] = random(64, 255);
      }
    }

    // shift all values in the matrix up one row
    void shiftUp() {
      for (uint8_t y = kMatrixHeight - 1; y > 0; y--) {
        for (uint8_t x = 0; x < kMatrixWidth; x++) {
          matrixValue[y][x] = matrixValue[ y - 1 ][ x ];
        }
      }
      for (uint8_t x = 0; x < kMatrixWidth; x++) {
        matrixValue[0][x] = line[x];
      }
    }

    /**
       draw a frame, interpolating between 2 "key frames"
      @param pcnt percentage of interpolation
    */
    void drawFrame(float pcnt) {
      int nextv;
      //each row interpolates with the one before it
      for (unsigned char y = kMatrixHeight - 1; y > 0; y--) {
        for (unsigned char x = 0; x < kMatrixWidth; x++) {
          nextv =
            (((100.0 - pcnt) * matrixValue[y][x]
              + pcnt * matrixValue[y - 1][x]) / 100.0)
            - valueMask[y][x];
          setXY(x, y, CHSV( hueMask[y][x], 255, (uint8_t)max(0, nextv)));
          //for ( uint8_t e = 0; e < kMatrixElements; e++) {
          //  setXYE(x, y,e, CHSV( hueMask[y][x]-2+(2*e), 255, (uint8_t)max(0, nextv)));
          //}

        }
      }

      //first row interpolates with the "next" line
      for (unsigned char x = 0; x < kMatrixWidth; x++) {
        setXY(x, 0, CHSV(
                hueMask[0][x], // H
                255,           // S
                (uint8_t)(((100.0 - pcnt)*matrixValue[0][x] + pcnt * line[x]) / 100.0) // V
              ));
      }
    }

    void fire_sparkle(){
      EVERY_N_MILLISECONDS(fire_sparkle_time_ms){
        if (random8(fire_sparkle_prob_over_per_time)==0) {
          setXYE(random8(kMatrixWidth),random8(kMatrixHeight/2,kMatrixHeight),1,CRGB::Orange);
          // one sparke comes seldome alone. 
          if (random8(fire_sparkle_prob_over_per_time / 2)==0) {
            setXYE(random8(kMatrixWidth),random8(kMatrixHeight/2,kMatrixHeight),1,CRGB::Orange);
          }
        }
      }
    }
};
