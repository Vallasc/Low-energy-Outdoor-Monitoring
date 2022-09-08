#include "http_manager.h"

bool HttpManager::begin(bool ping)
{
  char server[128];
  if (ping)
    snprintf(server, sizeof(server), "http://%s:%d/%s/%s/%s", host, port, ROUTE, id, ROUTE_CONFIG);
  else
    snprintf(server, sizeof(server), "http://%s:%d/%s/%s", host, port, ROUTE, id);
  client->begin(*wifiClient, server);
  char bearer[64];
  snprintf(bearer, sizeof(bearer), "Bearer  %s", token);
  client->addHeader("Authorization", bearer);
  return true;
}

int HttpManager::publish_sensors(float temp, float hum, float soil, float gas, int aqi, int wifi_rssi)
{
  Serial.println("Publish sensors HTTP");
  char payload[256];
  snprintf(payload, sizeof(payload), 
    "{\"temp\": %2f, "
    "\"hum\": %2f, "
    "\"soil\": %2f, "
    "\"gas\": %2f, "
    "\"aqi\": %d, "
    "\"rssi\": %d, "
    "\"latLong\": \"%lf,%lf\"}",
    temp, hum, soil, gas, aqi, wifi_rssi, latitude, longitude);
  int httpResponseCode = client->PUT(payload);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  if(httpResponseCode == 200)
  {
    strncpy(packet, client->getString().c_str(), sizeof(packet));
    init->init_from_json(String(packet));
  }
  return httpResponseCode;
}

bool HttpManager::update_config(
      unsigned long total_mqtt_packet_count, 
      unsigned long received_mqtt_packet_count,
      unsigned long total_http_packet_count,
      unsigned long received_http_packet_count,
      unsigned long mqtt_mean_time,
      unsigned long http_mean_times)
{
  // Send ping
  char payload[256];
  snprintf(payload, sizeof(payload),
                    "{\"id\": \"%s\", "
                      "\"token\": \"%s\", "
                      "\"host\": \"%s\", "
                      "\"wifiSsid\": \"%s\", "
                      "\"totalMqttPacketCount\": %lu, "
                      "\"receivedMqttPacketCount\": %lu, "
                      "\"totalHttpPacketCount\": %lu, "
                      "\"receivedHttpPacketCount\": %lu, "
                      "\"mqttMeanTime\": %lu, "
                      "\"httpMeanTime\": %lu }", 
                      id, token, host, wifi_ssid,
                      total_mqtt_packet_count, received_mqtt_packet_count, 
                      total_http_packet_count, received_http_packet_count, 
                      mqtt_mean_time, http_mean_times);
  int httpResponseCode = client->PUT(payload);
  Serial.println("Ping done HTTP: ");
  Serial.println(httpResponseCode);
  if(httpResponseCode == 200)
  {
    strncpy(packet, client->getString().c_str(), sizeof(packet));
    init->init_from_json(String(packet));
  }
  return httpResponseCode == 200;
}

bool HttpManager::update_config()
{
  char payload[256];
  snprintf(payload, sizeof(payload),
                        "{\"id\": \"%s\", "
                          "\"token\": \"%s\", "
                          "\"host\": \"%s\", "
                          "\"wifiSsid\": \"%s\"}", id, token, host, wifi_ssid);
  int httpResponseCode = client->PUT(payload);
  Serial.println("Ping done HTTP: ");
  Serial.println(httpResponseCode);
  if(httpResponseCode == 200)
  {
    strncpy(packet, client->getString().c_str(), sizeof(packet));
    init->init_from_json(String(packet));
  }
  return httpResponseCode == 200;
}