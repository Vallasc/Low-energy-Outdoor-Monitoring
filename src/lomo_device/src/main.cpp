#include <Arduino.h>
#include <DHT.h>
#include <Esp.h>

#include "mqtt_manager.h"
#include "init.h"
#include "sensors.h"
#include "config.h"
#include "state.h"
#include "wifi_utils.h"

WiFiClient wifiClient;
MQTTManager* mqtt_mng;
Sensors* sensors;

void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.println("LOMO v1.0");

  //preferences.clear();
  load_debug_config();
  load_config();

  if(!DEVICE_CONFIGURED)
  {
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

  print_config();

  if( !wifi_connect(WIFI_SSID.c_str(), WIFI_PASS.c_str()) )
    ESP.restart();

  mqtt_mng = new MQTTManager(&wifiClient, HOST.c_str(), MQTT_PORT, DEVICE_ID.c_str(), TOKEN.c_str());
  if( !mqtt_mng->connect() )
    ESP.restart();

  sensors = new Sensors();
}

void loop() {
  Serial.println("CIAO");
  //mqtt_mng->publish("hum", "39");
  mqtt_mng->publish_temperature(sensors->get_temperature());
  mqtt_mng->publish_humidity(sensors->get_humidity());
  Serial.println(sensors->get_soil());
  Serial.println(sensors->get_gas());
  delay(SAMPLE_FREQUENCY * 1000);
}