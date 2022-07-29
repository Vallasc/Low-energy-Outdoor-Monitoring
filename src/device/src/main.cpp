#include <Arduino.h>
#include <DHT.h>
#include <Esp.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>

#include "config.h"
#include "state.h"
#include "mqtt_manager.h"
#include "http_manager.h"
#include "init.h"
#include "sensors.h"
#include "wifi_utils.h"
#include "config_ping.h"
#include "evaluation.h"

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

void get_remote_config()
{
  InitServer init_tasks;
  ConfigPing* udp_config = new ConfigPing(&wifi_udp_client, HOST.c_str(), CONFIG_PORT, 
                                          DEVICE_ID.c_str(), TOKEN.c_str(), WIFI_SSID.c_str(), &init_tasks);
  udp_config->begin();
  if(PERFORMANCE_MONITORING)
    udp_config->update_config(
      total_mqtt_packet_count, 
      received_mqtt_packet_count,
      total_http_packet_count,
      received_http_packet_count,
      mqtt_mean_time,
      http_mean_times
    );
  else
    udp_config->update_config();
  delete(udp_config);
  Serial.println("Configuration changed");
  load_config();
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  if(strcmp(topic, mqtt_mng->get_full_topic()) == 0)
    STOP_EVALUATE_MQTT
}

void setup() {
  if(boot_count == 0)
    init_evaluation_vars();

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);

  delay(1000);
  Serial.begin(115200);
  delay(1000);
  Serial.println("LOMO v1.0");

  if(ENABLE_DEBUG)
    load_debug_config();
  load_config();

  if(!DEVICE_CONFIGURED){
    first_config();
    boot_count = 0;
    init_evaluation_vars();
  }
  print_config();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(BUTTON_PIN, isr, CHANGE);

  if( !wifi_connect(WIFI_SSID.c_str(), WIFI_PASS.c_str()) )
    ESP.restart();

  get_remote_config();
  boot_count += CONFIG_FREQUENCY;
  // Get remote device config
  if((boot_count - CONFIG_FREQUENCY) % SAMPLE_FREQUENCY != 0)
  {
    start_deepsleep(CONFIG_FREQUENCY);
    return; // useless
  }

  if(PROTOCOL_TYPE == "MQTT")
  {
    if(PERFORMANCE_MONITORING)
      mqtt_mng = new MQTTManager(&wifi_client, HOST.c_str(), MQTT_PORT, DEVICE_ID.c_str(), TOKEN.c_str(), mqtt_callback);
    else
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

  // fare subscribe mqtt tanto non funziona
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
    if(PERFORMANCE_MONITORING){
      int timeout = 5000;
      START_EVALUATE_MQTT(mqtt_mng->publish_sensors(temp, hum, soil, gas, aqi, rssi))
      while(--timeout > 0 && start_ms_mqtt != 0)
        mqtt_mng->loop();
    }
    else
    {
      mqtt_mng->publish_sensors(temp, hum, soil, gas, aqi, rssi);
    }
    delay(1000);
    mqtt_mng->disconnect();
    delete(mqtt_mng);
  }
  else
  {
    if(PERFORMANCE_MONITORING)
      EVALUATE_HTTP(http_mng->publish_sensors(temp, hum, soil, gas, aqi, rssi))
    else
      http_mng->publish_sensors(temp, hum, soil, gas, aqi, rssi);
    delete(http_mng);
  }
  print_stats();
  start_deepsleep(CONFIG_FREQUENCY);
}

void loop() {
  // Not used
}