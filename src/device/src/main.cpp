#include <Arduino.h>
#include <DHT.h>
#include <Esp.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>

#include "config.h"
#include "state.h"
#include "protocol_manager.h"
#include "mqtt_manager.h"
#include "http_manager.h"
#include "init.h"
#include "sensors.h"
#include "wifi_utils.h"
#include "config_ping.h"

 /* Conversion factor for micro seconds to minutes */
#define uS_TO_S_FACTOR 60000000ULL
RTC_DATA_ATTR unsigned long boot_count = 0;

WiFiClient wifi_client;
WiFiUDP wifi_udp_client;

HttpManager* http_mng;
MQTTManager* mqtt_mng;
Sensors* sensors;

void IRAM_ATTR isr()
{
  Serial.println("RESETTING DEVICE");
  reset_pref();
  ESP.restart();
}

void start_deepsleep(int minutes) 
{
  esp_sleep_enable_timer_wakeup(minutes * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(minutes) + " minutes");
  esp_deep_sleep_start();
}

void first_config()
{
  boot_count = 0;
  Serial.println("Waiting for initialization...");
  InitServer init_tasks;
  init_tasks.init();
  while(!init_tasks.is_inited())
  {
    init_tasks.handle_client();
    delay(500);
  }
  Serial.println("Device configured");
  delay(2000);
  ESP.restart();
}

void setup() {
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);

  delay(1000);
  Serial.begin(115200);
  delay(1000);
  Serial.println("LOMO v1.0");

  if(ENABLE_DEBUG)
    load_debug_config();
  load_config();

  if(!DEVICE_CONFIGURED)
    first_config();

  print_config();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(BUTTON_PIN, isr, FALLING);

  if( !wifi_connect(WIFI_SSID.c_str(), WIFI_PASS.c_str()) )
    ESP.restart();

  // Get remote device config
  if(boot_count % SAMPLE_FREQUENCY != 0)
  {
    InitServer init_tasks;
    ConfigPing* udp_config = new ConfigPing(&wifi_udp_client, HOST.c_str(), CONFIG_PORT, 
                                            DEVICE_ID.c_str(), TOKEN.c_str(), &init_tasks);
    udp_config->begin();
    udp_config->update_config();
    free(udp_config);
    Serial.println("Configuration changed");
    boot_count += CONFIG_FREQUENCY;
    start_deepsleep(CONFIG_FREQUENCY);
    return; // useless
  }

  if(PROTOCOL_TYPE == "MQTT")
  {
    mqtt_mng = new MQTTManager(&wifi_client, HOST.c_str(), MQTT_PORT, DEVICE_ID.c_str(), TOKEN.c_str());
    mqtt_mng->set_lat_long(LAT, LON);
    if( !((MQTTManager*)mqtt_mng)->begin() ){
      delete(mqtt_mng);
      ESP.restart();
    }
  }
  else
  {
    http_mng = new HttpManager(&wifi_client, HOST.c_str(), PROXY_PORT, DEVICE_ID.c_str(), TOKEN.c_str());
    http_mng->set_lat_long(LAT, LON);
    http_mng->begin();
  }

  sensors = new Sensors(MIN_GAS_VALUE, MAX_GAS_VALUE);
  sensors->begin();

  sensors->start();
  float temp = sensors->get_temperature();
  float hum = sensors->get_humidity();
  float soil = sensors->get_soil();
  float gas = sensors->get_gas();
  int aqi = sensors->get_aqi();
  sensors->stop();
  delete(sensors);

  int rssi = get_wifi_strength(3);

  if(PROTOCOL_TYPE == "MQTT")
  {
    mqtt_mng->publish_sensors(temp, hum, soil, gas, aqi, rssi);
    delay(1000);
    delete(mqtt_mng);
  }
  else
  {
    http_mng->publish_sensors(temp, hum, soil, gas, aqi, rssi);
    delay(1000);
    delete(http_mng);
  }

  boot_count += CONFIG_FREQUENCY;
  start_deepsleep(CONFIG_FREQUENCY);
}

void loop() {
  // Not used
}