#!/bin/bash
# Install the necessary build packages from the very beginning. 

echo
echo
echo ======================================================================
echo = Prepare the build environment
echo ======================================================================
echo
echo ======================================================================
echo = Step 1: Install Scons package
echo ======================================================================
echo
sudo apt-get install python3
sudo pip3 install scons
sudo ln -s -f /usr/bin/python3 /usr/bin/python
echo done.
echo
echo ======================================================================
echo = Step 2: Install GCC package
echo ======================================================================
echo
sudo apt-get install --yes --force-yes gcc
if [ $? -ne 0 ]; then 
    echo Error: sudo apt-get install gcc failed.
    exit 1
fi
echo done.
echo
echo
echo ======================================================================
echo = All the necessary packages have been installed. You can start to 
echo = play with examples. Just type: scons .
echo ======================================================================
echo

if [[ $1 = 'install' ]] ; then
    scons .
    sudo /bin/sh scripts/presence_detection_install.sh
fi

