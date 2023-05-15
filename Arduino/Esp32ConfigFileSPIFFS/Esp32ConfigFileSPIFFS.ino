#include <SPIFFS.h>
#include <ArduinoJson.h>    

/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true

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

  // Mount SPIFFS and read in config file
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
    Serial.println("setup -> SPIFFS Mount Failed");
  }
  else{
    Serial.println("setup -> SPIFFS mounted successfully");
    if(readConfig(config_filename) == false) {
      Serial.println("setup -> Could not read Config file -> initializing new file");
      // if not possible -> save new config file
      if (saveConfig(config_filename)) {
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
    
	if (saveConfig(config_filename)) {
    Serial.println("setup -> Config file saved");
  }   
	
	Serial.println("Value1 = " + String(value1) + ", value2 = " + String(value2) + ", user_string = " + user_string);
  }
}

bool readConfig(String file_name) {
  String file_content = readFile(SPIFFS, file_name);

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

bool saveConfig(String file_name) {
  StaticJsonDocument<1024> doc;

  // write variables to JSON file
  doc["value1"] = value1;
  doc["value2"] = value2;
  doc["user_string"] = user_string;
  
  // write config file
  String tmp = "";
  serializeJson(doc, tmp);
  writeFile(SPIFFS, config_filename, tmp);
  
  return true;
}

void writeFile(fs::FS &fs, String filename, String message){
  Serial.println("writeFile -> Writing file: " + filename);

  File file = fs.open(filename, FILE_WRITE);
  if(!file){
    Serial.println("writeFile -> failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("writeFile -> file written");
  } else {
    Serial.println("writeFile -> write failed");
  }
  file.close();
}

String readFile(fs::FS &fs, String filename){
  Serial.println("readFile -> Reading file: " + filename);

  File file = fs.open(filename);
  if(!file || file.isDirectory()){
    Serial.println("readFile -> failed to open file for reading");
    return "";
  }

  String fileText = "";
  while(file.available()){
    fileText = file.readString();
  }

  file.close();
  return fileText;
}
