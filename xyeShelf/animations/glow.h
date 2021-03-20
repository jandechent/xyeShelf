/**  _____ _                                _                 _   _             
 *  / ____| |                   /\         (_)               | | (_)            
 * | |  __| | _____      __    /  \   _ __  _ _ __ ___   __ _| |_ _  ___  _ __  
 * | | |_ | |/ _ \ \ /\ / /   / /\ \ | '_ \| | '_ ` _ \ / _` | __| |/ _ \| '_ \ 
 * | |__| | | (_) \ V  V /   / ____ \| | | | | | | | | | (_| | |_| | (_) | | | |
 *  \_____|_|\___/ \_/\_/   /_/    \_\_| |_|_|_| |_| |_|\__,_|\__|_|\___/|_| |_|
 *
 * by Jan Dechent 
 * as part of https://github.com/jandechent/xyeShelf
 * using GPL-3.0 License 
 * 
 * A glowing circly, gowing and shrinking. This animation doen't scale well for other xyeShelfs. 
 */

class xye_glow : public xye_animation {
  public:
    String get_animation_name() {return "Glow";};

    bool   set_parameter(byte remote_param){  
      param = remote_param;
      if (param == 0) { param = 1; }
      beat = float(param) / 16.0 ;
      return true; 
    }
    byte   get_parameter_value() { return param; }; // hopeully there are no severe rounding errors, that would decreas the beat over time...
    String get_parameter_desciption() { return this->get_animation_name() +"@"+String(beat) + " bpm"; };
    
    void setup(){
      this->set_parameter(4*16);
      FastLED.clear();
    }

    void loop(){
      glow_draw(2,3);
      FastLED.show();
    }

  private:
    float beat=0;
    byte param = 0;
    
    void glow_draw(int x0, int y0){
      uint8_t b = beatsin8(beat,0,128);      // 0 alles hell      // 64 ecken dunkel      // 128 unten schon hee
      for ( uint8_t y = 0; y < kMatrixHeight; y++) {
        for ( uint8_t x = 0; x < kMatrixWidth; x++) {
          float r = ((((x-x0)*(x-x0))+((y-y0)*(y-y0)))) / 8.0; // 0 to 8, but divided to keep soemwhat 0 to <1
          setXY(x,y,CHSV(175,255,cubicwave8(128+(b*r))));       
        }
      }    
    }
    /*
    float radius_inc = 0.025;
    float radius_min = radius_inc;
    float radius=radius_min;
    float radius_max = 2.25;
     
    void glow_draw(int x0, int y0){
      for ( uint8_t y = 0; y < kMatrixHeight; y++) {
        for ( uint8_t x = 0; x < kMatrixWidth; x++) {
          float dist_normed_f = ((((x-x0)*(x-x0))+((y-y0)*(y-y0))));
          dist_normed_f=(float)(dist_normed_f/(radius_max*radius_max));
          dist_normed_f=dist_normed_f/radius;
          if (dist_normed_f>=1){
            dist_normed_f=1;
          }

          int dist_normed=(int)255*dist_normed_f;
          setXY(x,y,CHSV(175,255,255-dist_normed));     
        }
      }  
      if (radius >= radius_max || radius <= radius_min){
        radius_inc=-radius_inc;
      }
      radius=radius-radius_inc;    
    };*/
};
