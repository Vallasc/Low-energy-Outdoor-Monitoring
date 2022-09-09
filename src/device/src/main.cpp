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
#include "udp_manager.h"
#include "evaluation.h"

 /* Conversion factor for micro seconds to minutes */
#define uS_TO_S_FACTOR 60000000ULL

RTC_DATA_ATTR unsigned long boot_count = 0;
RTC_DATA_ATTR unsigned long last_config_frequency = -1;
RTC_DATA_ATTR unsigned long last_sample_frequency = -1;
RTC_DATA_ATTR float avg_arr[5];
RTC_DATA_ATTR int aqi_index = 0;

InitServer init_tasks;

WiFiClient wifi_client;
WiFiUDP wifi_udp_client;

HttpManager* http_mng;
MQTTManager* mqtt_mng;
UdpManager* udp_manager;
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

void first_config();
void get_remote_config_http();
void get_remote_config_udp();

void check_config_frequency_change(){
  if(last_config_frequency != CONFIG_FREQUENCY || last_sample_frequency != SAMPLE_FREQUENCY)
  {
    last_config_frequency = CONFIG_FREQUENCY;
    last_sample_frequency = SAMPLE_FREQUENCY;
    boot_count = 0;
  }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  if(strcmp(topic, mqtt_mng->get_full_topic()) == 0)
    STOP_EVALUATE_MQTT
}

void setup() {
  delay(200);
  Serial.begin(115200);
  delay(200);
  Serial.println("LOMO v1.0");

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);
  // Load config from eeprom
  load_config();
  if(!DEVICE_CONFIGURED)
  {
    boot_count = 0;
    init_evaluation_vars();
    first_config();
  }
  if(!PERFORMANCE_MONITORING)
    init_evaluation_vars();

  // Force config on first boot
  if(boot_count == 0)
  {
    boot_count = CONFIG_FREQUENCY;
    for(int i=0; i<5; i++)
      avg_arr[i] = -1;
  }

  check_config_frequency_change();
  print_config();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(BUTTON_PIN, isr, CHANGE);

  if( !wifi_connect(WIFI_SSID.c_str(), WIFI_PASS.c_str()) )
    ESP.restart();

  int protocol;
  if(PROTOCOL_TYPE == "MQTT") protocol = 0;
  else if(PROTOCOL_TYPE == "HTTP") protocol = 1;
  else protocol = 2;

  if(boot_count % CONFIG_FREQUENCY == 0 && boot_count % SAMPLE_FREQUENCY != 0)
  {
    boot_count += CONFIG_FREQUENCY;
    switch (protocol)
    {
      case 0:
      case 1:
        get_remote_config_http();
        break;
      default:
        get_remote_config_udp();
        break;
    }
    start_deepsleep(CONFIG_FREQUENCY);
    // return;
  }
  boot_count += CONFIG_FREQUENCY;

  // Initialization
  switch (protocol)
  {
    case 0: // MQTT
      if(PERFORMANCE_MONITORING)
        mqtt_mng = new MQTTManager(&wifi_client, HOST.c_str(), MQTT_PORT, DEVICE_ID.c_str(), TOKEN.c_str(), mqtt_callback);
      else
        mqtt_mng = new MQTTManager(&wifi_client, HOST.c_str(), MQTT_PORT, DEVICE_ID.c_str(), TOKEN.c_str());
      mqtt_mng->set_lat_long(LAT, LON);
      if( !(mqtt_mng->begin()) )
        start_deepsleep(CONFIG_FREQUENCY);
      break;
    case 1: // HTTP
      http_mng = new HttpManager(&wifi_client, HOST.c_str(), PROXY_PORT, DEVICE_ID.c_str(), TOKEN.c_str(), WIFI_SSID.c_str(), &init_tasks);
      http_mng->set_lat_long(LAT, LON);
      http_mng->begin(false);
      break;
    default: // UDP
      udp_manager = new UdpManager(&wifi_udp_client, HOST.c_str(), CONFIG_PORT, 
                                          DEVICE_ID.c_str(), TOKEN.c_str(), WIFI_SSID.c_str(), &init_tasks);
      udp_manager->set_lat_long(LAT, LON);
      udp_manager->begin();
      break;
  }

  sensors = new Sensors(MIN_GAS_VALUE, MAX_GAS_VALUE, avg_arr, &aqi_index);
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

  switch (protocol)
  {
    case 0: // MQTT
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
      break;
    case 1: // HTTP
      if(PERFORMANCE_MONITORING)
        EVALUATE_HTTP(http_mng->publish_sensors(temp, hum, soil, gas, aqi, rssi))
      else
        http_mng->publish_sensors(temp, hum, soil, gas, aqi, rssi);
      delete(http_mng);
      break;
    case 2: // UDP
      udp_manager->publish_sensors(temp, hum, soil, gas, aqi, rssi);
      delete(udp_manager);
      break;
  }

  print_stats();
  start_deepsleep(CONFIG_FREQUENCY);
}

void loop() {
  // Not used
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

void get_remote_config_http()
{
  http_mng = new HttpManager(&wifi_client, HOST.c_str(), PROXY_PORT, DEVICE_ID.c_str(), TOKEN.c_str(), WIFI_SSID.c_str(), &init_tasks);
  http_mng->begin(true);
  if(PERFORMANCE_MONITORING)
    http_mng->update_config(
      total_mqtt_packet_count, 
      received_mqtt_packet_count,
      total_http_packet_count,
      received_http_packet_count,
      mqtt_mean_time,
      http_mean_times
    );
  else
    http_mng->update_config();
  delete(http_mng);
  load_config();
  Serial.println("Configuration changed");
}

void get_remote_config_udp()
{
  UdpManager* udp_manager = new UdpManager(&wifi_udp_client, HOST.c_str(), CONFIG_PORT, 
                                          DEVICE_ID.c_str(), TOKEN.c_str(), WIFI_SSID.c_str(), &init_tasks);
  udp_manager->begin();
  if(PERFORMANCE_MONITORING)
    udp_manager->update_config(
      total_mqtt_packet_count, 
      received_mqtt_packet_count,
      total_http_packet_count,
      received_http_packet_count,
      mqtt_mean_time,
      http_mean_times
    );
  else
    udp_manager->update_config();
  delete(udp_manager);
  load_config();
  Serial.println("Configuration changed");
}