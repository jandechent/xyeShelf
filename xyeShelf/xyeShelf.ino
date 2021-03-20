/*   _______ _             __   __ __     __  ______      _____ _          _  __
 *  |__   __| |            \ \ / / \ \   / / |  ____|    / ____| |        | |/ _|
 *     | |  | |__   ___     \ V /   \ \_/ /  | |__      | (___ | |__   ___| | |_
 *     | |  | '_ \ / _ \     > <     \   /   |  __|      \___ \| '_ \ / _ \ |  _|
 *     | |  | | | |  __/    / . \     | |    | |____     ____) | | | |  __/ | |
 *     |_|  |_| |_|\___|   /_/ \_\    |_|    |______|   |_____/|_| |_|\___|_|_|
 * --------------------------------------------------------------------------------
 * by Jan Dechent 
 * as part of https://github.com/jandechent/xyeShelf
 * using GPL-3.0 License 
 */

#define DEMO 0

#if DEMO == 0
  const uint8_t kMatrixElements = 4; // my arduino has 4 elements per pocket. 
  #include "animations/sprites/sprite_tree.h"
  #include "animations/sprites/sprite_font1.h"
#else
  #warning Demo mode active, this do not work on my real hardware. Potentially, include other files in this file, before adding to the simulator. 
  const uint8_t kMatrixElements = 1; //but we use a reduced number of leds to play it a reduced set
#endif

#define FASTLED_INTERNAL
#include <FastLED.h>
#include "common.h"

const uint8_t elementsWidth = 30; // cm to keep in integer
const uint8_t elementsHeight = 28; // cm to keep in integer

const uint8_t kMatrixWidth = 5;
const uint8_t kMatrixHeight = 6;

const uint8_t NUM_LEDS = kMatrixWidth * kMatrixHeight * kMatrixElements; 

unsigned char line[kMatrixWidth]; // originally from fire animation, but put to global scope, to reuse it for other animations and save memory. 
unsigned char matrixValue[kMatrixHeight][kMatrixWidth]; // some animations need a scaled down matrix to store one uint8_t (like hue or so) with a "low res" version, which is then applied to all elements

CRGB leds[NUM_LEDS];
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B

#define DATA_PIN 3
//#define BUTTON_0 9
//#define BUTTON_1 10
//#define BUTTON_2 11

struct xy_pair {
  int8_t x;
  int8_t y;
  xy_pair& operator+(const xy_pair& other){ 
    x += other.x;
    y += other.y;
    return *this;
  };
};

//      /\      | |   | |                 (_)             | |    |  ____|  __ \
//     /  \   __| | __| |_ __ ___  ___ ___ _ _ __   __ _  | |    | |__  | |  | |___
//    / /\ \ / _` |/ _` | '__/ _ \/ __/ __| | '_ \ / _` | | |    |  __| | |  | / __|
//   / ____ \ (_| | (_| | | |  __/\__ \__ \ | | | | (_| | | |____| |____| |__| \__ \
//  /_/    \_\__,_|\__,_|_|  \___||___/___/_|_| |_|\__, | |______|______|_____/|___/
//                                                   _/ |
//                                                 |___/

/*
// This function wil be used by the colorutils, which must see the x dimention as a combination of x and e. 
// threfore we must pretent to not have elements, but they are part of x...
uint16_t XY(uint8_t xe, uint8_t y) { 
  // TERRIBLE HACK, whic of course doen not work, due to const and we cant change const due to leds...
  uint8_t real_kMatrixElements = kMatrixElements;
  kMatrixElements=1;
  uint8_t i = XYE(xe,y,0); 
  kMatrixElements=real_kMatrixElements;
  return i
}
*/

uint16_t XY0(uint8_t x, uint8_t y) { return XYE(x,y,0); }

uint16_t XYE( uint8_t x, uint8_t y, uint8_t e){
  uint16_t i;
  uint8_t reverseX = (kMatrixWidth - 1) - x;
  uint8_t reverseE =  (kMatrixElements - 1) - e;
  #if DEMO <= 1 
    // (0,0) in the lower left and the strip starts in the lower left via snake wiring upwards.
    // Use in conjunction with the real hardware (DEMO=0) or respective simulator and DEMO=1. 
    if ( y & 0x01) { // Odd rows run backwards
      i = (y * kMatrixElements * kMatrixWidth) + (reverseX * kMatrixElements) + reverseE;
    }
    else { // Even rows run forwards
      i = ( y * kMatrixElements * kMatrixWidth ) + ( x * kMatrixElements ) + e;
    }
  #else 
    // (0,0) in the lower left, but strip starts in the uppper left and goes downwards. 
    // Use this for DEMO=2 and the respective simulator. 
    i = ( kMatrixWidth * kMatrixElements ) * ( kMatrixHeight - y ) - 1 - reverseX - reverseE;
  #endif
  return i;
}

uint16_t setXYE( uint8_t x, uint8_t y, uint8_t e, CRGB c) { leds[XYE(x, y, e)] = c; }

uint16_t setXY( uint8_t x, uint8_t y, CRGB c) {
  if ( y & 0x01 ) { //due to snake wiring, even and odd rows need to handled separately
    fill_solid( &(leds[XYE(x, y, kMatrixElements-1)]), kMatrixElements, c);
  }else{
    fill_solid( &(leds[XYE(x, y, 0)]), kMatrixElements, c);
  }
}

uint16_t setXY_lr( uint8_t x, uint8_t y, CRGB c, int8_t lr)
{
  uint8_t start = 0;
  uint8_t end = kMatrixElements / 2;
  if (lr == 1) {
    start = (kMatrixElements / 2);
    end = kMatrixElements;
  }
  // also here, a fill_solid would work perfectly well, might increase performance.
  for (uint8_t e = start ; e < end; e++) {
    leds[XYE(x, y, e)] = c;
  }
}

void set_sprite(const uint8_t sprite[][kMatrixHeight][kMatrixWidth][3], uint8_t frame){
   // this really should be using memcopy, but then every second row in the sprite array needs reversing and the elements need to be multiplied ... for the online simulator not....
  for ( uint8_t y = 0; y < kMatrixHeight; y++) {
    for ( uint8_t x = 0; x < kMatrixWidth; x++) {
      //uint8_t alpha = pgm_read_word(&sprite[frame][y][x][3]);
      //if (alpha==255){
        //Serial.print(y);Serial.print(",");Serial.print(x);Serial.print(" : ");
        uint8_t r = pgm_read_word(&sprite[frame][y][x][0]);
        uint8_t g = pgm_read_word(&sprite[frame][y][x][1]);
        uint8_t b = pgm_read_word(&sprite[frame][y][x][2]);
        //Serial.print(r);Serial.print("-");Serial.print(g);Serial.print("-");Serial.print(b);
        //Serial.println("");
        setXY(x,kMatrixHeight-1-y, CRGB (r,g,b));
      //}
    }
  }
}

void show_text(String text){
  for(int i =0; i < text.length(); i++ ) {
    byte sprite_pos = text[i]-65;
    for (uint8_t b=0 ; b<64 ; b++){
      set_sprite(sprite_font1,sprite_pos);
      FastLED.setBrightness(cubicwave8(2*b));
      FastLED.show();
      FastLED.delay(2);
    }
    FastLED.delay(800);
  }
}

uint16_t ggt(uint16_t a, uint16_t b){ //https://de.wikipedia.org/wiki/Euklidischer_Algorithmus#Iterative_Variante
  uint16_t h; 
  while ( b!=0 ){ h = a % b; a = b; b = h; }
  return a;
}

//   _____       _                   ______                _   _
//  |  __ \     | |                 |  ____|              | | (_)
//  | |  | | ___| |__  _   _  __ _  | |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//  | |  | |/ _ \ '_ \| | | |/ _` | |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//  | |__| |  __/ |_) | |_| | (_| | | |  | |_| | | | | (__| |_| | (_) | | | \__ \
//  |_____/ \___|_.__/ \__,_|\__, | |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                            __/ |
//                           |___/

// XYE_Debug(0, 0, 0);   XYE_Debug(1, 0, 0);   XYE_Debug(1, 0, 1);   XYE_Debug(0, 1, 0);   XYE_Debug(1, 1, 0);   XYE_Debug(1, 1, 1);
void XYE_Debug( uint8_t x, uint8_t y, uint8_t e){  Serial.print("x=");  Serial.print(x);  Serial.print("; y=");  Serial.print(y);  Serial.print("; e=");  Serial.print(e);  Serial.print("; -->i=");  Serial.println(XYE(x, y, e)); }

void basicStats() {
  Serial.print("DEMO mode       = ");  Serial.print(DEMO);
  switch( DEMO ) { 
    case 0: Serial.println(" => real hardware mode"); break;
    case 1: Serial.println(" => online simulator 1"); break;
    case 2: Serial.println(" => online simulator 2"); break;
  }
  Serial.print("kMatrixWidth    = ");  Serial.println(kMatrixWidth);
  Serial.print("kMatrixHeight   = ");  Serial.println(kMatrixHeight);
  Serial.print("kMatrixElements = ");  Serial.println(kMatrixElements);
  Serial.print("Number of LEDs  = ");  Serial.println(NUM_LEDS);
}

//    _____      _                          _
//   / ____|    | |                 ___    | |
//  | (___   ___| |_ _   _ _ __    ( _ )   | |     ___   ___  _ __
//   \___ \ / _ \ __| | | | '_ \   / _ \/\ | |    / _ \ / _ \| '_ \
//   ____) |  __/ |_| |_| | |_) | | (_>  < | |___| (_) | (_) | |_) |
//  |_____/ \___|\__|\__,_| .__/   \___/\/ |______\___/ \___/| .__/
//                        | |                                | |
//                        |_|                                |_|

// Prorotype Class, to handle the different animations. 
class xye_animation {
  public:
    virtual void setup() {};
    virtual void loop()  {};
    virtual String get_animation_name() {return "";};
    virtual byte   get_parameter_value() {return 0;};
    virtual String get_parameter_desciption() {return this->get_animation_name();};
    virtual bool   set_parameter(byte remote_param) {return false;};
};

void send_answer(int state, String description){
  Serial.print("{\"state\": \"" );
  Serial.print(state);
  Serial.print("\",\"description\": \"");
  Serial.print(description);
  Serial.println("\"}");
}

const uint8_t num_xye_animation=7;
#include "animations/fire.h"
#include "animations/pacifica.h"
#include "animations/glow.h"
#include "animations/tree.h"
#include "animations/pong.h"
#include "animations/disco.h"
#include "animations/flow.h"
#include "animations/happybirthday.h"

xye_animation *animations[num_xye_animation];
int command = 0;

void process_command(){
  byte remote_command = Serial.read(); //at least two bytes are available for sure
  byte remote_payload = Serial.read();
  
  //Serial.println("--- processing command ---");
  //Serial.print  ("echoing received command: "); Serial.println(remote_command);
  //Serial.print  ("echoing received payload: "); Serial.println(remote_payload);
  
  switch (remote_command){
    case CMD_SET_ANIMATION: 
      set_animation(remote_payload); break;
    case CMD_SET_PARAMETER: 
      set_parameter(remote_payload); break;
    case CMD_GET_PARAMETER:
      send_answer(animations[command]->get_parameter_value(),animations[command]->get_parameter_desciption()); break;
    case CMD_SET_BRIGHTNESS:
      FastLED.setBrightness(remote_payload); send_answer(remote_payload,""); break;
    default:
      send_answer(-1,"unknown/out of bounds");
  }
}

void set_animation(byte requested_animation){
  if (requested_animation < num_xye_animation){
    command = requested_animation;
    animations[command]->setup();
    send_answer(requested_animation,animations[command]->get_animation_name());
  }else{
    command = 0;
    send_answer(-1,"unknown/out of bounds");
  }
}

void set_parameter(byte remote_parameter){
  if (animations[command]->set_parameter(remote_parameter)){
    send_answer(remote_parameter,animations[command]->get_parameter_desciption());
  }else{
    send_answer(-1,animations[command]->get_parameter_desciption());
  }
}

void loop() {
  if (Serial.available()>=2) { process_command(); }
  animations[command]->loop();
}

void setup() {
  Serial.begin(38400);
  randomSeed(analogRead(0)); // go get different sequence on reboots. 
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.clear();  FastLED.show();

  uint8_t i = 0;
  animations[i++] = (xye_animation*) new xye_fire();
  animations[i++] = (xye_animation*) new xye_pacifica();
  //animations[i++] = (xye_animation*) new xye_glow();
  //animations[i++] = (xye_animation*) new xye_tree();
  animations[i++] = (xye_animation*) new xye_pong();
  animations[i++] = (xye_animation*) new xye_disco();
  animations[i++] = (xye_animation*) new xye_flow();
  //animations[i++] = (xye_animation*) new xye_happybday();
  if ((i+1) > num_xye_animation){ Serial.println("Error 03: animations arrow had overflown, check sketch!"); }
  
  animations[command]->setup();
}
