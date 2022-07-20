#include <Arduino.h>
#include <DHT.h>
#include <Esp.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>

#include "protocol_manager.h"
#include "mqtt_manager.h"
#include "coap_manager.h"
#include "init.h"
#include "sensors.h"
#include "config.h"
#include "state.h"
#include "wifi_utils.h"

#include <coap-simple.h>
WiFiClient wifi_client;
WiFiUDP wifi_udp_client;

CoAPManager* coap_mng;
MQTTManager* mqtt_mng;

// TODO free resources
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
    while(!init_tasks.is_inited())
    {
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

  if(PROTOCOL_TYPE == "MQTT")
  {
    mqtt_mng = new MQTTManager(&wifi_client, HOST.c_str(), MQTT_PORT, DEVICE_ID.c_str(), TOKEN.c_str());
    if( !((MQTTManager*)mqtt_mng)->begin() )
      ESP.restart();
  }
  else
  {
    coap_mng = new CoAPManager(&wifi_udp_client, HOST.c_str(), 5683, TOKEN.c_str());
    coap_mng->begin();
  }

  sensors = new Sensors(MIN_GAS_VALUE, MAX_GAS_VALUE);
  sensors->begin();
}

void loop() {
  Serial.println("CIAO");

  sensors->start();
  float temp = sensors->get_temperature();
  float hum = sensors->get_humidity();
  float soil = sensors->get_soil();
  float gas = sensors->get_gas();
  int aqi = sensors->get_aqi();
  sensors->stop();

  int rssi = get_wifi_strength(3);

  // ADD lat long
  if(PROTOCOL_TYPE == "MQTT")
    mqtt_mng->publish_sensors(temp, hum, soil, gas, aqi, rssi);
  else
    coap_mng->publish_sensors(temp, hum, soil, gas, aqi, rssi);
  
  delay(SAMPLE_FREQUENCY * 1000);
}