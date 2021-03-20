/*  _____                  
 * |  __ \                 
 * | |__) |__  _ __   __ _ 
 * |  ___/ _ \| '_ \ / _` |
 * | |  | (_) | | | | (_| |
 * |_|   \___/|_| |_|\__, |
 *                    __/ |
 *                   |___/ 
 * by Jan Dechent 
 * as part of https://github.com/jandechent/xyeShelf
 * using GPL-3.0 License 
 */

class xye_pong : public xye_animation {
  public:
    String get_animation_name() {return "Pong";};
    String get_parameter_desciption() { 
      int delta = this->wins_bottom - this->wins_up;
      float delta_prozent = float(delta) / float(this->wins_up + this->wins_bottom) * 100;
      String sign = "";
      if (delta > 0) { sign = "+";}
      return this->get_animation_name() +"; "+
        String(this->wins_up) +":"+String(this->wins_bottom) + 
        " (" +sign+String(delta_prozent) +"%)"; 
    };
    byte get_parameter_value() {return param;};
    bool set_parameter(byte remote_param) {
      param = remote_param;
      // blance goes between -1 to 1 indicating the balance between players
      balance = float(remote_param-128) / float(128);
      // propability to follow at balance point for both players. 
      uint8_t pong_follow = 204; // = p*255; 204 is 80%; do not choose less than 128, otherwise one of the balances is negative, but actually overflows. 
      // probablity for each of the players to span up to 255. 
      prob_balance_up     = pong_follow + ( -balance * ( 255-pong_follow ) );
      prob_balance_bottom = pong_follow + ( +balance * ( 255-pong_follow ) );
      return true;
    };

    void setup(){
      FastLED.clear();
      pong_state=0;
      pong_ball.x= random8(kMatrixWidth);
      pong_ball.y = kMatrixHeight/2;
      pong_speed.x = +1;
      pong_speed.y = -1;
    }

    void loop(){
      EVERY_N_MILLISECONDS(pong_updatespeed){
          pong_step();
      };
      EVERY_N_MINUTES(30){randomSeed(analogRead(0));} // pong will run for a while, not sure, how often this is ideal. 
      FastLED.show();
    }

  private:
    byte param=128;
    float balance = 0;
    uint8_t prob_balance_up = 0;
    uint8_t prob_balance_bottom = 0;
    
    int pong_updatespeed_initial=500;
    int pong_updatespeed_increment = -25; // increment, when the ball is reflected. 
    int pong_updatespeed = pong_updatespeed_initial;

    struct xy_pair pong_ball;
    struct xy_pair pong_speed;

    int8_t pong_top_x;
    int8_t pong_bottom_x;
    int8_t pong_state = 0; // 0 playing, -1 bottom won, 1 top won

    uint16_t wins_up = 0;
    uint16_t wins_bottom = 0;

    void pong_step(){
      if (pong_state == 0 ){
        pong_play();
      }else if(pong_state==1){
        pong_won(true);
      }else if(pong_state==-1){
        pong_won(false);
      }
    }

    void pong_won(bool up){
      if (up){
        wins_up++;
        if (wins_up==0){wins_bottom==0;} // overflow resets the other too
      }else{
        wins_bottom++;
        if (wins_bottom==0){wins_up==0;} // overflow resets the other too
      }
      pong_state=0; // we could go for fancy animations =)
      
      FastLED.clear();
      pong_updatespeed = pong_updatespeed_initial;
      pong_ball.x = random(kMatrixWidth); // because the ball moves only diagonally, we need to randomzize here - otherwise we end up with fixexd asymetric patterns all the time, making the game unfair for one player. 
      if (random8(2) == 0){ // flip x speed randomly 50:50
        pong_speed.x = -pong_speed.x;
      }
      
    }

    void pong_play(){
      // test advance the ball, so we can detect reflections and update pong_speed
      struct xy_pair newpos;
      newpos.x = pong_ball.x + pong_speed.x;
      newpos.y = pong_ball.y + pong_speed.y;

      if((newpos.x >= kMatrixWidth) || (newpos.x <= -1)) { pong_speed.x = -pong_speed.x; }
      if((newpos.y >= kMatrixHeight)|| (newpos.y <= -1)) { pong_speed.y = -pong_speed.y; }
      
      fadeToBlackBy(leds, kMatrixWidth*kMatrixHeight*kMatrixElements, 120);
      setXY(pong_top_x,kMatrixHeight-1,CRGB(0,0,0));
      setXY(pong_bottom_x,0,CRGB(0,0,0));
      
      // actually advance the ball
      pong_ball=pong_ball+pong_speed;

      if (prob_balance_up>random8(255)) {
        if(pong_top_x-pong_ball.x>0) { pong_top_x--;}
        if(pong_top_x-pong_ball.x<0) { pong_top_x++;}
      }
      if (prob_balance_bottom>random8(255)) {
        if(pong_bottom_x-pong_ball.x>0) { pong_bottom_x--;}
        if(pong_bottom_x-pong_ball.x<0) { pong_bottom_x++;}
      }
      
      // draw the players in blue, might be overwritten later but maybe not. 
      setXY(pong_top_x,kMatrixHeight-1,CRGB(0,0,255));
      setXY(pong_bottom_x,0,CRGB(0,0,255));

      if (pong_ball.y == kMatrixHeight-1){ // pong hit the top
        if (pong_top_x == pong_ball.x){ // top player got the ball
          setXY(pong_ball.x,pong_ball.y,CRGB(0,255,0)); // draw in green
          pong_updatespeed=max(5,pong_updatespeed+pong_updatespeed_increment);
        }else{ // top player missed, bottom won
          fill_solid(leds,NUM_LEDS/2,CRGB(0,255,0)); // signal, who won
          fill_solid(&leds[NUM_LEDS/2],NUM_LEDS/2,CRGB(255,0,0));
          pong_state = -1;
        }
      }else if (pong_ball.y == 0){ // pong hit the bottom
        if (pong_bottom_x == pong_ball.x){ // bottom player go the ball
          setXY(pong_ball.x,pong_ball.y,CRGB(0,255,0));
          pong_updatespeed=max(5,pong_updatespeed+pong_updatespeed_increment);
        }else{ // bottom player missed, top won
          fill_solid(leds,NUM_LEDS/2,CRGB(255,0,0)); // signal, who won
          fill_solid(&leds[NUM_LEDS/2],NUM_LEDS/2,CRGB(0,255,0));
          pong_state = 1;
        }
      }else{ // the ball moves along the playing field
        setXY(pong_ball.x,pong_ball.y,CRGB(255,255,255));
      }
    }
};
