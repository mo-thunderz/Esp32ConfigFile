/* IMPORTANT
  This code uses the built in LittleFS library in the ESP32 library. Please verify that the ESP32 library is R2.0.4 or newer for
  this code to run. To do so, go to Tools -> Board -> Boards Manager and search for "ESP32". You should see the esp32 library from
  Espressif Systems. Make sure the version is 2.0.4 or newer. */

#include "FS.h"
#include <LittleFS.h>
#include <ArduinoJson.h>  

/* You only need to format LITTLEFS the first time you run a
   test or else use the LITTLEFS plugin to create a partition
   https://github.com/lorol/arduino-esp32littlefs-plugin */
   
#define FORMAT_LITTLEFS_IF_FAILED true

// define filename to store config file
const String config_filename = "/config.json";

// initialize the variables of the program
int value1 = 0;
int value2 = 0;
String user_string = "";

void setup() {
  // Start serial interface
  Serial.begin(115200);
  Serial.println("-----------------------------Start demo SPIFFS-----------------------------");

  // print initialized variables
  Serial.println("setup -> Before reading config: Value1 = " + String(value1) + ", value2 = " + String(value2));

  // Mount LITTLEFS and read in config file
  if (!LittleFS.begin(false)) {
    Serial.println("LITTLEFS Mount failed");
    Serial.println("Kein Dateisystemsystem gefunden; wird formatiert");
    // falls es nicht klappt, erneut mit Neu-Formatierung versuchen
    if (!LittleFS.begin(true)) {
      Serial.println("LITTLEFS Mount fehlgeschlagen");
      Serial.println("Formatierung nicht möglich");
      return;
    } else {
      Serial.println("Formatierung des Dateisystems erfolgt");
    }
  }
  else{
    Serial.println("setup -> SPIFFS mounted successfully");
    if(readConfig() == false) {
      Serial.println("setup -> Could not read Config file -> initializing new file");
      // if not possible -> save new config file
      if (saveConfig()) {
        Serial.println("setup -> Config file saved");
      }   
    }

    Serial.println("Value1 = " + String(value1) + ", value2 = " + String(value2) + ", user_string = " + user_string);
  }
}

void loop() {
  // Check if user sent a new string
  if (Serial.available() > 0)
  {
    user_string = Serial.readString(); //Die empfangenen Bytes werden in einer Variable gespeichert
	
	// generate new values for our variables:
  value1 = random(1000);
  value2 = random(1000);
    
	if (saveConfig()) {
    Serial.println("setup -> Config file saved");
  }   
	
	Serial.println("Value1 = " + String(value1) + ", value2 = " + String(value2) + ", user_string = " + user_string);
  }
}

bool readConfig() {
  String file_content = readFile(config_filename);

  int config_file_size = file_content.length();
  Serial.println("Config file size: " + String(config_file_size));

  if(config_file_size > 1024) {
    Serial.println("Config file too large");
    return false;
  }
  StaticJsonDocument<1024> doc;
  auto error = deserializeJson(doc, file_content);
  if ( error ) { 
    Serial.println("Error interpreting config file");
    return false;
  }

  const int _value1 = doc["value1"];
  const int _value2 = doc["value2"];
  const String _user_string = doc["user_string"];

  value1 = _value1;
  value2 = _value2;
  user_string = _user_string;
  return true;
}

bool saveConfig() {
  StaticJsonDocument<1024> doc;

  // write variables to JSON file
  doc["value1"] = value1;
  doc["value2"] = value2;
  doc["user_string"] = user_string;
  
  // write config file
  String tmp = "";
  serializeJson(doc, tmp);
  writeFile(config_filename, tmp);
  
  return true;
}

void writeFile(String filename, String message){
    File file = LittleFS.open(filename, "w");
    if(!file){
        Serial.println("writeFile -> failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

String readFile(String filename){
    File file = LittleFS.open(filename);
    if(!file){
        Serial.println("Failed to open file for reading");
        return "";
    }
    
    String fileText = "";
    while(file.available()){
        String fileText = file.readString();
    }
    file.close();
    return fileText;
}
