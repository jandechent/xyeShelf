#!/bin/bash

echo "stopping xyeShelf's MQTT engine.."
ssh pi@192.168.178.3 sudo systemctl stop xyeShelf_mqtt.service

echo "Uploading sketch to remote... "
rsync -Par . pi@192.168.178.3:~/arduino/xyeShelf

