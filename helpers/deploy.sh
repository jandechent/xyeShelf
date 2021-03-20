#!/bin/bash

echo "stopping xyeShelf's MQTT engine.."
ssh pi@192.168.178.3 sudo systemctl stop xyeShelf_mqtt.service

echo "Uploading sketch to remote... "
rsync -Par . pi@192.168.178.3:~/arduino/xyeShelf

echo ""
echo "Starting remote compile, followed by upload to board:"
ssh pi@192.168.178.3 /home/pi/arduino/arduino-cli compile -v -p /dev/ttyUSB0 -b arduino:avr:nano:cpu=atmega328old /home/pi/arduino/xyeShelf/xyeShelf/
ssh pi@192.168.178.3 /home/pi/arduino/arduino-cli upload -v -p /dev/ttyUSB0 -b arduino:avr:nano:cpu=atmega328old /home/pi/arduino/xyeShelf/xyeShelf/

echo "starting xyeShelf's MQTT engine.."
ssh pi@192.168.178.3 sudo systemctl start xyeShelf_mqtt.service
