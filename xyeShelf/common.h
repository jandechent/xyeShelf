#define CMD_SET_ANIMATION 0
#define CMD_SET_PARAMETER 1
#define CMD_GET_PARAMETER 2
#define CMD_SET_BRIGHTNESS 3
#define CMD_GET_BRIGHTNESS 4

#define E01_MqttDown "Error 1: python mqtt middleware is down"
#define E02_NoSer "Error 2: no serial connection"

// errors on basic communication
#define E10_TimeOut "Error 10: connection timed out"
#define E11_AnimBnd "Error 11: animations arrow had overflown, check sketch!"

// errors within passed parameters
#define E21_NoNumber "Error 21: passed parameter is no number"
#define E22_OutBounds "Error 22: passed parameter is out of bounds"
