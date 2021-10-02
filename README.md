# xyeShelf
<img align="right" width="400" height="400" src="https://jandechent.de/images/xyeShelf.jpg">
A MQTT controlled, FastLED powered Arduino sketch to animate each pocket of a shelf, having multiple WS2812B's. There are multiple animations that can be run:

 - sparkling **fire** animation, inspired by [NeoFire](https://github.com/mic159/NeoFire/blob/master/NeoFire.ino).
 - blue wave **Pacifica** animation, inspired by this [FastLED example](https://github.com/FastLED/FastLED/blob/master/examples/Pacifica/Pacifica.ino).
 - glowing **ball**, expanding and collapsing periodically. 
 - growing tree animation. 
 - a game of **pong**, where the players are blue, the ball white. When a player hits the ball they light up green. When a player misses, the hemispheres light up green and red to indicate the player that lost or won. 
 - colorful **disco** animation, where the beats per minute can be controlled via the parameter. 
 - two white dots, **flow**ing in circles. 




## MQTT Control
The ```xyeShelf_mqtt.py``` starts a server, through which you an control the xyeShelf. The following MQTT topics are relevant:

 - ```/xyeShelf/reboot``` to publish a 1 to, to reboot the arduino board.
 - ```/xyeShelf/animation_requested``` to publish a integer number to, to choose an animation.
 - ```/xyeShelf/parameter_requested``` to publish a integer number (0 to 255) to, to control a certain aspect of the animation.
 
|   | Animation | Parameter |
|:-:| --------- | --------- |
| 0 | fire      | Control the liveliness of the animation. The ```+‰```-value indicates, how the mixing ratio of two key frames is increased with each loop. When the ratio is at 100%, the next key frame is calculated. |
| 1 | pacifica  | no effect |
| 2 | ball      | no effect |
| 3 | tree      | no effect |
| 4 | pong      | At 128, the game is balanced for a tie between the players, so both miss the ball equally often. <br>At 0 or 255, one player never misses, the other double as much, compared to balance. | 
| 5 | disco     | Control the beats per minute (```bpm```), as p/2=bpm. So when p=255, the bpm is 128. |
| 6 | flow      | no effect |

Further, other mqtt clients can subscribe to the following topics to see, what is going on on the board:

 - ```/xyeShelf/animation_actual``` to subscribe to, to see the result of requesting an animation.
 - ```/xyeShelf/parameter_actual``` to subscribe to, to see the result of requesting a parameter.

Both ```actual``` topics will contain a json response in the following form:
```
   {
     "state": "<as accepted by sketch or negative for errors>",
     "description": "<textual description for the state>"
   }
```
The script needs to be run on the device, that the arduino is connected to via USB. Please adjust the server in the python script to your needs. 

## Further Reading
- [xyeShelf sketch](https://github.com/jandechent/xyeShelf/blob/master/xyeShelf/xyeShelf.md)
