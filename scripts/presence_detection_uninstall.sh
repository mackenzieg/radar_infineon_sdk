#!/bin/bash
# Uninstall HMI Presnece Detection Service


INIT_SERVICES=/etc/init.d
SYS_SERVICES=/etc/systemd/system

sudo systemctl stop presence_detection.service;
sudo systemctl disable presence_detection.service;

sudo rm -rf $SYS_SERVICES/presence_detection*
sudo rm -rf $INIT_SERVICES/presence_detection*

sudo systemctl daemon-reload;
sudo systemctl reset-failed;