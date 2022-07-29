#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <HTTPClient.h>

#define ROUTE "/devices"

class HttpManager 
{
  public:
    HttpManager(WiFiClient* _wifiClient, const char* _host, const int _port, const char* _id, const char* _token)
    {
      client = new HTTPClient();
      wifiClient = _wifiClient;
      strcpy(host, _host);
      port = _port;
      strcpy(id, _id);
      strcpy(token, _token);
      perf_monitor = false;
    }

    ~HttpManager()
    {
      client->end();
      delete (client);
    }

    bool begin();

    void set_lat_long(double lat, double lon)
    {
      this->latitude = lat;
      this->longitude = lon;
    }

    int publish_sensors(float temp, float hum, float soil, float gas, int aqi, int wifi_rssi);

  private:
    HTTPClient* client;
    WiFiClient* wifiClient;
    WiFiUDP* wifi_udp_client;

    char host[32];
    int port;
    char token[64];
    char id[64];
    double latitude;
    double longitude;
    bool perf_monitor;
};

#endif