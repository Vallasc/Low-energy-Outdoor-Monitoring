#ifndef COAPMANAGER_H
#define COAPMANAGER_H

#include <WiFiUdp.h>
#include <coap-simple.h>
#include "protocol_manager.h"

#define COAP_URL "devices" 
class CoAPManager : public ProtocolManager {
  public:
    Coap *client;
    CoAPManager(WiFiUDP* wifiClient, const char* _host, const int _port, const char* _token)
    {
      client = new Coap(*wifiClient);
      strcpy(host, _host);
      port =_port;
      strcpy(token, _token);

      address.fromString(host);
      //client->response(std::bind(&CoAPManager::callback_response_config, this));
    }

    ~CoAPManager()
    {
      delete (client);
    }

    bool begin()
    {
      client->start();
      return true;
    }
    void publish_sensors(float temp, float hum, float soil, float aqi);
    void get_config();
    void callback_response_config(CoapPacket &packet, IPAddress ip, int port);

  private:
    char host[32];
    int port;
    char token[64];
    IPAddress address;
};

#endif