# The xyeShelf

Animating leds in my self (5 columns, 6 rows), using 4 WS2815B led per pocket.
FastLED serves as foundation library, using a arduino uno compatible board.

## Physical Shelf Setup:

The layout of the shelf is as follows:
```
            ┼────┼────┼────┼────┼────┼
  DATA┌─>   │    |x1y2|    |x3y2|    |            >══╗
   wir│     ┼────┼────┼────┼────┼────┼               ║
   ing└─<   |    |x2y1|    |    |    |  <─┐DATA   >══╬═> +12V / GND (WS2515B)
            ┼────┼────┼────┼────┼────┼    │wir       ║
  DATAPin─> |x0y0|x1y0|    |    |    |  >─┘ing    >══╝
            ┼────┼────┼────┼────┼────┼

One Pocket:    ┼────────────────┼
               │   x=1;  y=2    |
               │  with n leds   |
               │ e(0),..,e(n-1) |
               ┼────────────────┼
```

 - ```kMatrixWidth``` is the total number of pockets in horizontal direction the variable
   for this direction is x and starts from 0
 - ```kMatrixHeight``` is the total number of pockets in vertical direction the variable
   for this direction is y and starts from 0
 - ```kMatrixElements``` is the total number of leds inside of each pocket the variable
   for this is e is n and starts from 0

## Low level Functions:

Each pocket of the shelf contains a certain number of leds, which can be individually adrresses or at once. All low level function work for any sized shelf and  number of leds per pocket.
 - ```setXY (x,y,c)```       to set all elements of on one  pocket x,y to the color c
 - ```setXY_lr(x,y,c,dir)``` to set all the left sided leds to c (dir=-1), or right (dir=1)
 - ```setXYE (x,y,e,c)```    to set one specific led (element) in the pocket x,y to the color c
 - ```XYE (x,y,e)```          to get the indice of the specific led, respecting the wiring as below.
 - ```XY0 (x,y)```             calls XYE(x,y,0) for the use in online simluatory using one led per self pocket.

## Leds:

Here, WS2515B were used and wiring was setup to supply wach row with +12/GND from the source. To minimize the length of data transmitting cables, a snakelike wiring was chosen, as shown. When using WS2815B, remember to connect the ground from the board to the ground on the 12V side. Also a resistor between the board and first DI was used, as well as a capacitor to stabilize the 12V power source.

## For my shelf:

 - BI and BO: small red wires
 - first DI: small organge wire
 - DI and DO: small white wires
 - 12V and GND: large white wires
 - GND between strip and arduino: brown wire

## Transitioning to Your Project 
All animations will work on any number of pockets and elements per pocket. You have to adapt the relevant variables in the main sketch and probably the ```XYE(x,y,e)``` and ```XY(x,y)``` function. Everything relies on these functions, so yes, performance could be improved my using memcopy.

## Adding New Animations
Derive a new class based off ```class xye_animation```. 

## For (online) demos:

there are tools to simulate, what the code would do. This is neat for playing around, and debugging. Use the following settings:
 - 0 => use for physical hardware
 - 1 => use lesson.playground and https://wokwi.com/playground/fastled
 - 2 => use diagramm.playground and https://wokwi.com/arduino/libraries/FastLED/LEDFace
