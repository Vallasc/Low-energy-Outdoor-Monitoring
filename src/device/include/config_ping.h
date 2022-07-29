#ifndef CONFIG_PING_H
#define CONFIG_PING_H

#include <WiFiUdp.h>
#include "init.h"

#define LOCAL_PORT 4210

class ConfigPing
{
  public:
    ConfigPing(WiFiUDP* _udp_client, const char* _host, int _remote_port, const char* _id, const char* _token, const char* _ssid, InitServer* _init)
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
      unsigned long http_mean_times)
    {
      // Send ping
      udp_client->beginPacket(host, remote_port);
      udp_client->printf("{\"id\": \"%s\", "
                          "\"token\": \"%s\", "
                          "\"host\": \"%s\", "
                          "\"wifiSsid\": \"%s\", "
                          "\"totalMqttPacketCount\": %lu, "
                          "\"receivedMqttPacketCount\": %lu, "
                          "\"totalHttpPacketCount\": %lu, "
                          "\"receivedHttpPacketCount\": %lu, "
                          "\"mqttMeanTime\": %lu, "
                          "\"httpMeanTime\": %lu }", 
                          id, token, host, ssid,
                          total_mqtt_packet_count, received_mqtt_packet_count, 
                          total_http_packet_count, received_http_packet_count, 
                          mqtt_mean_time, http_mean_times);
      udp_client->endPacket();
      Serial.println("Ping done");
      return read_response();
    }

    bool update_config()
    {
      // Send ping
      udp_client->beginPacket(host, remote_port);
      udp_client->printf("{\"id\": \"%s\", "
                          "\"token\": \"%s\", "
                          "\"host\": \"%s\", "
                          "\"wifiSsid\": \"%s\"}", id, token, host, ssid);
      udp_client->endPacket();
      Serial.println("Ping done");
      return read_response();
    }

    bool read_response()
    {
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
    char token[64];
    char packet[255];
    char host[32];
    char ssid[32];
    int remote_port;
    InitServer* init;
};

#endif