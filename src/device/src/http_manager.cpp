#include "http_manager.h"

bool HttpManager::begin()
{
  char server[64];
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
  return httpResponseCode;
}