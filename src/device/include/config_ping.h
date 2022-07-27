#ifndef CONFIG_PING_H
#define CONFIG_PING_H

#include <WiFiUdp.h>
#include "init.h"

#define LOCAL_PORT 4210

class ConfigPing
{
  public:
    ConfigPing(WiFiUDP* _udp_client, const char* _host, int _remote_port, const char* _id, const char* _token, InitServer* _init)
    {
      udp_client = _udp_client;
      strcpy(id, _id);
      strcpy(token, _token);
      strcpy(host, _host);
      remote_port= _remote_port;
      init = _init;
    }

    void begin()
    {
      udp_client->begin(LOCAL_PORT);
    }

    bool update_config()
    {
      // Send ping
      udp_client->beginPacket(host, remote_port);
      udp_client->printf("{\"id\": \"%s\", \"token\": \"%s\"}", id, token);
      udp_client->endPacket();
      Serial.println("Ping done");

      bool readPacket = false;
      int retries = 10;
      while (!readPacket && retries-- > 0)
      {
        int packetSize = udp_client->parsePacket();
        if (packetSize)
        {
          // receive incoming UDP packets
          Serial.printf("Received %d bytes from %s, port %d\n", 
                packetSize, udp_client->remoteIP().toString().c_str(), udp_client->remotePort());
          int len = udp_client->read(packet, sizeof(packet));
          if (len > 0)
          {
            packet[len] = 0;
          }
          Serial.printf("UDP packet contents: %s\n", packet);
          readPacket = true;
        } else {
          delay(500);
        }
      }
      if(readPacket)
        init->init_from_json(String(packet));
      
      return readPacket;
    }

  private:
    WiFiUDP *udp_client;
    char id[32];
    char token[32];
    char packet[255];
    char host[32];
    int remote_port;
    InitServer* init;
};

#endif