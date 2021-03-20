/*    _____                   _               _______                             _                 _   _
 *   / ____|                 (_)             |__   __|                /\         (_)               | | (_)
 *  | |  __ _ __ _____      ___ _ __   __ _     | |_ __ ___  ___     /  \   _ __  _ _ __ ___   __ _| |_ _  ___  _ __
 *  | | |_ | '__/ _ \ \ /\ / / | '_ \ / _` |    | | '__/ _ \/ _ \   / /\ \ | '_ \| | '_ ` _ \ / _` | __| |/ _ \| '_ \
 *  | |__| | | | (_) \ V  V /| | | | | (_| |    | | | |  __/  __/  / ____ \| | | | | | | | | | (_| | |_| | (_) | | | |
 *   \_____|_|  \___/ \_/\_/ |_|_| |_|\__, |    |_|_|  \___|\___| /_/    \_\_| |_|_|_| |_| |_|\__,_|\__|_|\___/|_| |_|
 *                                     __/ |
 *                                    |___/
 *
 * by Jan Dechent 
 * as part of https://github.com/jandechent/xyeShelf
 * using GPL-3.0 
 */

class xye_tree : public xye_animation {
  public:
    String get_animation_name() {return "Baum";};

    void setup(){ FastLED.clear(); };

    void loop(){
      EVERY_N_MILLISECONDS(500){
        set_sprite(sprite_tree,frame%sprite_tree_length);
        frame++;
        FastLED.show();
      }
    }
    
  private:
    uint8_t frame = 0;

};
