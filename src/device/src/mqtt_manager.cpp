#include "mqtt_manager.h"

bool MQTTManager::begin()
{
  int i = this->max_mqtt_retries;
  while (!client->connected() && i-- > 0)
  {
    Serial.println("Connecting to MQTT broker ");
    if (client->connect(device_id, device_id, token))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.println(client->state());
      delay(5000);
    }
  }
  if (!client->connected())
  {
    Serial.print("Broker not connected: max mqtt retries reached.");
    return false;
  }
  return true;
}

void MQTTManager::publish_sensors(float temp, float hum, float soil, float gas, int aqi, int wifi_rssi)
{
  Serial.println("Publish sensors MQTT");
  char payload[256];
  snprintf(payload, sizeof(payload), "%.2f/%.2f/%.2f/%.2f/%d/%d/%lf,%lf", temp, hum, soil, gas, aqi, wifi_rssi, latitude, longitude);
  Serial.println(payload);
  client->publish(full_topic, payload);
}

bool MQTTManager::is_connected()
{
  return client->connected();
}