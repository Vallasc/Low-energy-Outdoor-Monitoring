#include <Arduino.h>
#include <Esp.h>
#include "init.h"
#include "state.h"


void setup2() {
  delay(2000);
  Serial.begin(9600);
  Serial.println("LOMO v1.0");

  Preferences* preferences = open_preferences();
  preferences->putBool(DEVICE_CONFIGURED, true);
  close_preferences();


  preferences = open_preferences();
  Serial.println("- Device configured: " + String(preferences->getBool(DEVICE_CONFIGURED, false)));
  close_preferences();
}


void setup() {
  delay(2000);
  Serial.begin(9600);
  Serial.println("LOMO v1.0");

  open_preferences();
  //preferences.clear();
  //preferences.putBool(DEVICE_CONFIGURED, false);
  bool device_configured = preferences.getBool(DEVICE_CONFIGURED, false);
  String protocol = preferences.getString(PROTOCOL_TYPE, "");
  close_preferences();

  if(!device_configured){
    Serial.println("Waiting for initialization...");
    InitAPWebServer init_tasks;
    init_tasks.init();
    while(!init_tasks.is_inited()){
      init_tasks.handle_client();
      delay(500);
    }
    Serial.println("Device configured");
    print_config();
    delay(2000);
    ESP.restart();
  }

  // pinMode(21, OUTPUT);
  // digitalWrite(21, HIGH);
  
  // pinMode(22, OUTPUT);
  // digitalWrite(22, HIGH);
}

void loop() {
  Serial.println("CIAO");
  delay(1000);
}