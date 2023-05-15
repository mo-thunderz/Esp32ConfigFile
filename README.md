# ESP32 Config File stored on with either LittleFS or SPIFFS
In this code some parameters are stored on the internal built SD card in the ESP32 (referred to commonly as SPIFFS). This is very useful in applications where you want to store some parameters that should survive a reboot. 

# IMPORTANT
If you use ESP board library V2.04 or newer, please use LittleFS, if you are using an older ESP32 library you will have to use SPIFFS. In the Arduino IDE you can check what ESP board library you are using by navigating to: Tools -> Board -> Boards Manager 

# Youtube
Explanation on Youtube is for SPIFFS, but the LittleFS version works just the same: https://youtu.be/Q3vV3MdOxAU
