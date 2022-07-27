#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <HTTPClient.h>
#include "protocol_manager.h"

#define ROUTE "/devices"

class HttpManager : public ProtocolManager {
  public:
    HttpManager(WiFiClient* _wifiClient, const char* _host, const int _port, const char* _id, const char* _token)
    {
      client = new HTTPClient();
      wifiClient = _wifiClient;
      strcpy(host, _host);
      port = _port;
      strcpy(id, _id);
      strcpy(token, _token);

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

    void publish_sensors(float temp, float hum, float soil, float gas, int aqi, int wifi_rssi);

  private:
    HTTPClient* client;
    WiFiClient* wifiClient;

    char host[32];
    int port;
    char token[64];
    char id[64];
    double latitude;
    double longitude;
};

#endif