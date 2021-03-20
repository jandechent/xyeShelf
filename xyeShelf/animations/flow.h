/*  ______ _               
 * |  ____| |              
 * | |__  | | _____      __
 * |  __| | |/ _ \ \ /\ / /
 * | |    | | (_) \ V  V / 
 * |_|    |_|\___/ \_/\_/  
 * 
 * by Jan Dechent 
 * as part of https://github.com/jandechent/xyeShelf
 * using GPL-3.0 
 */

class xye_flow : public xye_animation {
  public:
    String get_animation_name() {return "Fluss";};
  
    void setup(){
      for (uint8_t i=0;i< xye_flow::cf_NUM;i++){
        setXY(this->cf_circle[i][0],this->cf_circle[i][1],CRGB(0,0,0));
      }
    };

    void loop(){
      EVERY_N_MILLISECONDS(20) {
        climbflow_circle_loop();
        FastLED.show();
      }
    };

  private:
    static const int8_t cf_NUM=14;
    float cf_step=0;

    /*uint8_t cf_circle[cf_NUM][2]= {
      {0,0},{1,0},{2,0},{3,0},{4,0},  
      {4,1},{4,2},{4,3},{4,4},
      {4,5},{3,5},{2,5},{1,5},{0,5},
      {0,4},{0,3},{0,2},{0,1}
    };*/

    uint8_t cf_circle[xye_flow::cf_NUM][2]= {
      {1,0},{2,0},{3,0},
      {4,1},{4,2},{4,3},{4,4},
      {3,5},{2,5},{1,5},
      {0,4},{0,3},{0,2},{0,1}
    };

    void climbflow_circle_loop(){
      for (uint8_t k=0; k < xye_flow::cf_NUM ; k++){
        uint8_t i =  ( k + xye_flow::cf_NUM ) % cf_NUM ;
        uint8_t v = cubicwave8(((float)2*255.0*k/cf_NUM)+this->cf_step);
        setXY(this->cf_circle[i][0],this->cf_circle[i][1],CHSV(96,0,v));
      }
      cf_step+=1;
    };
  
};
