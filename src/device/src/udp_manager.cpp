#include "udp_manager.h"
bool UdpManager::update_config(
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
                      "\"ping\": true, "
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

bool UdpManager::update_config()
{
  // Send ping
  udp_client->beginPacket(host, remote_port);
  udp_client->printf("{\"id\": \"%s\", "
                      "\"token\": \"%s\", "
                      "\"host\": \"%s\", "
                      "\"ping\": true, "
                      "\"wifiSsid\": \"%s\"}", id, token, host, ssid);
  udp_client->endPacket();
  Serial.println("Ping done");
  return read_response();
}

bool UdpManager::read_response()
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

void UdpManager::publish_sensors(float temp, float hum, float soil, float gas, int aqi, int wifi_rssi)
{
  Serial.println("Publish sensors UDP");
  udp_client->beginPacket(host, remote_port);
  udp_client->printf("{\"id\": \"%s\", "
                      "\"token\": \"%s\", "
                      "\"temp\": %2f, "
                      "\"hum\": %2f, "
                      "\"soil\": %2f, "
                      "\"gas\": %2f, "
                      "\"aqi\": %d, "
                      "\"rssi\": %d, "
                      "\"ping\": false, "
                      "\"latLong\": \"%lf,%lf\"}",
                      id, token, temp, hum, soil, gas, aqi, wifi_rssi, latitude, longitude);
  udp_client->endPacket();
}