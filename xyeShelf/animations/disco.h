/** _____  _               
 * |  __ \(_)              
 * | |  | |_ ___  ___ ___  
 * | |  | | / __|/ __/ _ \ 
 * | |__| | \__ \ (_| (_) |
 * |_____/|_|___/\___\___/ 
 * 
 * by Jan Dechent 
 * as part of https://github.com/jandechent/xyeShelf
 * using GPL-3.0 
 */
 
class xye_disco : public xye_animation {
  public:
    String get_animation_name() { return "Disco"; };
    bool   set_parameter(byte remote_param){  
      beat = float(remote_param) / 2.0 ;
      if (beat == 0) { beat = 1; }
      return true; 
    }

    byte   get_parameter_value() { return 2*beat; }; // hopeully there are no rounding errors, that would decreas the beat over time...
    String get_parameter_desciption() { return this->get_animation_name() +"@"+String(beat) + " bpm"; };
    
    void loop(){
      uint8_t v = beatsin8(beat,75,255);
      uint8_t shift = beatsin8(4,0,255);
      for ( uint8_t y = 0; y < kMatrixHeight; y++) {
        for ( uint8_t x = 0; x < kMatrixWidth; x++) {
          matrixValue[y][x] = ( ( (2*(x+1)) * (12*(y+1)) ) + shift );
          setXY(x,y,CHSV(matrixValue[y][x],255,v));
        }
      }
      FastLED.show();
    };
    
  private:
    float beat=50;
    
};
