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
 
Basic colors
===

Color info when booting:
 * Blue: System is booting, and no network is available.
 * Green: System is ready, WiFi is connected, and you change colors

Color code when doing an OTA update:
 * Purple: System started an OTA update
 * Gold/Yellowish: System is done with the update and will reboot soon (And the colors will change to the boot colors!)
 * Red: A error happened.

Used examples
===
Not all code in this project is just written by me. Some of the code is based of others, like:
 * OTA code based from https://educ8s.tv/esp8266-ota-wemos-d1/
 * WebServer code based from https://randomnerdtutorials.com/esp8266-web-server/