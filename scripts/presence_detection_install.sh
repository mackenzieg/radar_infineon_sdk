#!/bin/bash
# Install the presence detection demo as service. 

DIST_PATH=/home/pi/RadarSDK
INIT_SERVICES=/etc/init.d
SYS_SERVICES=/etc/systemd/system

# install as service
sudo cp $DIST_PATH/scripts/presence_detection.sh $INIT_SERVICES
sudo chmod a+rx $INIT_SERVICES/presence_detection.sh
sudo cp $DIST_PATH/scripts/presence_detection.service $SYS_SERVICES

sudo systemctl disable presence_detection.service;
sudo systemctl enable presence_detection.service;
sudo systemctl start presence_detection.service;