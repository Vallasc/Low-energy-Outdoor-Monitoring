#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <DHT.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "config.h"

#define MQTT_PUB_QOS 3
#define MQTTsubQos 1

// /devies/foo
#define MAIN_MQTT_TOPIC "devices"

class MQTTManager
{
public:
  MQTTManager(WiFiClient* _wifiClient, const char* _host, int port, const char* _device_id, const char* _token)
  {
    client = new PubSubClient(*_wifiClient);
    strcpy(host, _host);
    client->setServer(host, port);
    strcpy(device_id, _device_id);
    strcpy(token, _token);
    snprintf(full_topic, sizeof(full_topic), "%s/%s", MAIN_MQTT_TOPIC, device_id);
  }

  MQTTManager(WiFiClient* _wifiClient, const char* _host, int port, const char* _device_id, const char* _token, std::function<void(char*, uint8_t*, unsigned int)> _callback)
  {
    client = new PubSubClient(*_wifiClient);
    strcpy(host, _host);
    client->setServer(host, port);
    strcpy(device_id, _device_id);
    strcpy(token, _token);
    snprintf(full_topic, sizeof(full_topic), "%s/%s", MAIN_MQTT_TOPIC, device_id);
    callback = _callback;
  }

  ~MQTTManager()
  {
    delete (client);
  }

  bool begin();
  void set_lat_long(double lat, double lon)
  {
    this->latitude = lat;
    this->longitude = lon;
  }

  bool is_connected();
  char* get_full_topic()
  {
    return full_topic;
  }

  void loop()
  {
    client->loop();
  }

  void disconnect()
  {
    client->disconnect();
  }

  void publish_sensors(float temp, float hum, float soil, float gas, int aqi, int wifi_rssi);
  
private:
  PubSubClient* client;
  
  char host[32];
  char device_id[64];
  char token[64];
  char full_topic[64];
  const int max_mqtt_retries = 5;
  double latitude;
  double longitude;
  std::function<void(char*, uint8_t*, unsigned int)> callback;
};

#endif