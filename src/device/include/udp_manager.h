#ifndef UDP_MANAGER_H
#define UDP_MANAGER_H

#include <WiFiUdp.h>
#include "init.h"

#define LOCAL_PORT 4210

class UdpManager
{
  public:
    UdpManager(WiFiUDP* _udp_client, const char* _host, int _remote_port, const char* _id, const char* _token, const char* _ssid, InitServer* _init)
    {
      udp_client = _udp_client;
      strncpy(id, _id, sizeof(id));
      strncpy(token, _token, sizeof(token));
      strncpy(host, _host,sizeof(host));
      strncpy(ssid, _ssid, sizeof(ssid));
      remote_port= _remote_port;
      init = _init;
    }

    void begin()
    {
      udp_client->begin(LOCAL_PORT);
    }

    bool update_config(
      unsigned long total_mqtt_packet_count, 
      unsigned long received_mqtt_packet_count,
      unsigned long total_http_packet_count,
      unsigned long received_http_packet_count,
      unsigned long mqtt_mean_time,
      unsigned long http_mean_times);
    bool update_config();
    bool read_response();
    void set_lat_long(double lat, double lon)
    {
      this->latitude = lat;
      this->longitude = lon;
    }
    void publish_sensors(float temp, float hum, float soil, float gas, int aqi, int wifi_rssi);
  private:
    WiFiUDP *udp_client;
    InitServer* init;

    char id[32];
    char token[64];
    char packet[255];
    char host[32];
    char ssid[32];
    int remote_port;
    double latitude;
    double longitude;
};

#endif