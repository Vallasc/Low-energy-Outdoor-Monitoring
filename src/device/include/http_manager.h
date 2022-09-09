#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <HTTPClient.h>
#include "init.h"

#define ROUTE "devices"
#define ROUTE_CONFIG "config"

class HttpManager 
{
  public:
    HttpManager(WiFiClient* _wifiClient, const char* _host, const int _port, const char* _id, const char* _token, const char* _wifi_ssid, InitServer* _init)
    {
      client = new HTTPClient();
      wifiClient = _wifiClient;
      strncpy(host, _host, sizeof(host));
      port = _port;
      strncpy(id, _id, sizeof(id));
      strncpy(token, _token, sizeof(token));
      strncpy(wifi_ssid, _wifi_ssid, sizeof(wifi_ssid));
      perf_monitor = false;
      init = _init;
    }

    ~HttpManager()
    {
      client->end();
      delete (client);
    }

    bool begin(bool ping);

    void set_lat_long(double lat, double lon)
    {
      this->latitude = lat;
      this->longitude = lon;
    }

    bool update_config(
      unsigned long total_mqtt_packet_count, 
      unsigned long received_mqtt_packet_count,
      unsigned long total_http_packet_count,
      unsigned long received_http_packet_count,
      unsigned long mqtt_mean_time,
      unsigned long http_mean_times);
    bool update_config();

    int publish_sensors(float temp, float hum, float soil, float gas, int aqi, int wifi_rssi);

  private:
    HTTPClient* client;
    WiFiClient* wifiClient;
    WiFiUDP* wifi_udp_client;
    InitServer* init;

    char host[32];
    char wifi_ssid[32];
    int port;
    char token[64];
    char id[64];
    char packet[512];
    double latitude;
    double longitude;
    bool perf_monitor;
};

#endif