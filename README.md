The zipped file Ifx_RadarSDK_<version>_<date>_<git commit hash value>_<platform>.zip 
(e.g. Ifx_RadarSDK_v1.0.0_20190517_24f66f0e_raspi.zip) is the name of the Radar SDK release package.

Before starting, please make sure that Rapberry Pi has internet connection and check default python 
is python3. Or simply try: "ln -s -f /usr/bin/python3 /usr/bin/python". To install the Radar SDK,
execute the following steps:

1. Extract the zip file to a folder: /home/pi/RadarSDK.
2. Right-click the “RadarSDK” folder and select open in terminal.
3. Then execute the command in the opened terminal: ./bootstrap.sh. This script prepares the build 
   environment and checks for required SW packages which are needed for compiling and/or running the
   SDK. The installation can take from 30 to 120 seconds. 
4. Start directly with: "scons ." and all the examples in the example folder are built.
5. Go to example/<name_example>, and run "./build/<name_example>" to start a specific example
   application.
6. To run presence detection application directly, skip points 3-5 and run the command 
   "./bootstrap.sh install".  It would install and run the presence detection application as a service
   in the background. 
 
Note: Currently the scripts inside 'data' folder are only valid for preparing the presence detection 
      application as a service. If you want to run another example, user must uninstall this service. 
      Because it occupies the radar hardware.

For more information, please check the "IFX_RadarSDK_Installation_Guide_v1.1" as reference.
