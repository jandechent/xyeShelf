# MQTT middleware, that forwards commands to the arduino via serial and sends back the result
# - publish a command to topic xyeShelf/command
# - subscribe to topic xyeShelf/response to see arduinos response

from __future__ import print_function
import paho.mqtt.client as mqtt
import sys
import serial
import time
import argparse
import json
import re

CMD_SET_ANIMATION = 0
CMD_SET_PARAMETER = 1
CMD_GET_PARAMETER = 2
CMD_SET_BRIGHTNESS = 3
CMD_GET_BRIGHTNESS = 4

#common_file = file1 = open('../xyeShelf/common.h', 'r')
#common={}
#for line in common_file:
#    define_line = re.search(r'#define\s*([^\n\r]*)', line)
#    if define_line:
#    	theline=define_line.group(1)
#        name = re.search(r'^(.*?)\s', theline).group(1)
#        value = re.search(r'\S*([^\n\r]*)', theline).group(1)[1:]
#        if value[0]=='"':
#            common[name]=str(value[1:-1])        
#        else:
#            print(value)
#            common[name]=int(value)
#print(common["CMD_SET_BRIGHTNESS"])
# get defines with re: #define\s*([^\n\r]*)
# ^\S*
# 

def serial_connect():
    try:
        global s
        s = serial.Serial('/dev/ttyUSB0', 38400, timeout=1)
        s.open()
        serial_port = "Open"
    except serial.serialutil.SerialException:
        print("pi  : Port needs another resetting.")
        s.close()
        s.open()
    time.sleep(1) # arduino resets in that time. Typically goes faster, but 1 seconds does the job.
    print("pi  : Serial connection should now be open.")
    return True


def serial_send(command,payload):
    print("pi  : starting serial send");
    try:   
        payload=int(payload)
    except:
        return answer(-1,"error, please pass over a number")    
        
    if (payload<0) | (payload>255):
        return answer(-1,"error, pass over number between 0 and 255")
    
    if s.isOpen() == False:  # s must exist, but this is the case, because we initialize in on_connect.
        serial_connect()
        print("pi  : it was necessary to reconnect...")
    
    print("pi  : sending command as byte array:" +str(command)+str(payload))
    s.write(bytearray([command, payload]))
    print("pi  : transmitted, waiting for response")

    start = time.time()
    while time.time() - start < 2:  # seconds to wait for response, quiet long... 
        response = s.readline()
        print("nano: " + response, end='')
        try:
	   response_string = json.loads(response)
	   print("pi  : retrieved valid JSON, passing string over.")
           return response
        except:
	   pass
    print("\npi  : timeout, while waiting for response")
    return answer(-1,"timeout, while waiting for response")


def on_connect(client, userdata, flags, rc):
    print("pi  : Connected with result code " + str(rc))
    if serial_connect():
        client.subscribe("xyeShelf/animation_requested")
        client.subscribe("xyeShelf/parameter_requested")
        client.subscribe("xyeShelf/brightness_requested")
        client.publish("xyeShelf/reboot",0)
        client.subscribe("xyeShelf/reboot")
        client.publish('xyeShelf/animation_requested', 0) # sketch initalizes to animation[0], but to keep everything consistent, we need to init from here. 
        client.publish('xyeShelf/brightness_requested', 255) # sketch initalizes to max bightness, but to keep everything consistent, we need to init from here.
    else:
        client.publish('xyeShelf/parameter_actual', answer(-1,'connection failed'))
        time.sleep(2)  # We settle a bit, to prevent high CPU load, when the arduino is e.g. just not conencted. 
        client.reconnect()


def answer(state,description):
    return '{"state":"'+str(state)+'","description":"'+description+'"}'


def on_message(client, userdata, msg):
    if msg.topic == 'xyeShelf/animation_requested':
        client.publish('xyeShelf/animation_actual', serial_send(CMD_SET_ANIMATION,msg.payload))
        time.sleep(0.01) # not sure, why we would have to wait here even.
        parameter = json.loads(serial_send(CMD_GET_PARAMETER,0)) # retrieve the parameter, that the animation puts itself into during it's setup
        client.publish('xyeShelf/parameter_requested', parameter["state"]) # resend this via the topic to bring the system into one consitent state. 
    if msg.topic == 'xyeShelf/parameter_requested':
        client.publish('xyeShelf/parameter_actual', serial_send(CMD_SET_PARAMETER,msg.payload))
    if msg.topic == 'xyeShelf/brightness_requested':
	client.publish('xyeShelf/brightness_actual', serial_send(CMD_SET_BRIGHTNESS,msg.payload))
    if msg.topic == 'xyeShelf/reboot':
        if msg.payload == '1':
            print("closing serial port")
            s.close();
            serial_connect();
            client.publish('xyeShelf/reboot',0)
        

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.will_set("xyeShelf/parameter_actual", payload=answer(-1,'xyeShelf_mqtt.py middleware died'), qos=2, retain=False)
client.connect('192.168.178.3', 1883, 60)

start_time = time.time();
parameter_response_previous=""
while True:
    client.loop()
    if ( ( time.time() - start_time ) > 5 ):
        start_time = time.time()
        print("pi  : Periodic update to parameter_actual")
        parameter_response_current=serial_send(CMD_GET_PARAMETER,0)
        if (parameter_response_current != parameter_response_previous): # to keep mqtt communication low, we check if the response is new
            client.publish('xyeShelf/parameter_actual', parameter_response_current ) 
            parameter_response_previous = parameter_response_current



