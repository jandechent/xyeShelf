/**  Happy Birthday
 *
 * by Jan Dechent 
 * as part of https://github.com/jandechent/xyeShelf
 * using GPL-3.0 License 
 * 
 * A glowing circly, gowing and shrinking. This animation doen't scale well for other xyeShelfs. 
 */

class xye_happybday : public xye_animation {
  public:
    String get_animation_name() {return "HappyBDay";};
    
    void setup(){
      FastLED.clear();
    }

    void loop(){
      show_text("Happy Birthday");
    }

};
