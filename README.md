Simple LED strip controller
===

Some basic code to control a (short) LED strip.
If it has a network connection already known, it will use that.
Otherwise, it will start a new Access Point, to which you should connect.
(The password is below in the settings). 
From there you can define which network should be used.
 
Once it is connected, you can browse to the IP it was assigned 
(Unfortently, it is not easy to show that to you, so you will need to check your router!)
and change the color of the LEDs.

Please note that I won't be able to provide real support for this project. 
If you find bugs, you are free to report them in the issue tracker, however I can't promise that I will be able to fix them.
 
Basic colors
===
Color info when booting:
 * Blue: System is booting, and no network is available.
 * Green: System is ready, WiFi is connected, and you change colors

Color code when doing an OTA update:
 * Purple: System started an OTA update
 * Gold/Yellowish: System is done with the update and will reboot soon (And the colors will change to the boot colors!)
 * Red: A error happened.

Required parts
===
This project is meant to display a few LEDs in a 3D printed object. Depending on what you exactly want to do you need the following parts:
 * A wemos D1 MINI (This is kind of required :))
 * A LED strip with the needed amount of LEDs. If you don't use WS2812B LEDs, you will need to update the settings in main.cpp
 * A power source. I used a Adafruit PowerBoost 500 Charger + 2500mAh LiPo battery. Depending on your project you can also just use a 5v USB power supply. 

Installation
===
This project require platformio. You should follow the installation instructions on their site to get started.
After opening the project you can start programming your weimos d1 mini.
Before flashing the firmware, make sure to check the settings on top of the file. If needed, update them to match your need.

If you haven't flashed the firmware before, you will need to do it via USB. Connect the wemos to your PC via a USB port.
You might need to remove the upload_protocol and upload_port properties from the platformio.ini file.
After this, upload the firmware directly to your microcontroller.

Once you flahsed your firmware, OTA updates have been enabled. 
Add to you platformio.ini file the following settings:
```
upload_protocol = espota
upload_port = 10.42.42.10
```
where 10.42.42.10 is the IP of your microcontroller. After that you can directly flash your microcontroller over your WiFi network.
If you did set the hostname correctly, you can also use that as upload_port for the OTA update.

Used examples
===
Not all code in this project is just written by me. Some of the code is based of others, like:
 * OTA code based from https://educ8s.tv/esp8266-ota-wemos-d1/
 * WebServer code based from https://randomnerdtutorials.com/esp8266-web-server/