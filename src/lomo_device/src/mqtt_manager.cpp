#include "mqtt_manager.h"

bool MQTTManager::begin()
{
  int i = this->max_mqtt_retries;
  while (!client->connected() && i-- > 0)
  {
    Serial.print("Connecting to MQTT broker ");
    if (client->connect(device_id, device_id, token))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client->state());
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

void MQTTManager::publish_temperature(float value)
{
  char buff[8];
  snprintf(buff, sizeof(buff), "%.2f", value);
  publish(TEMP, buff);
}

void MQTTManager::publish_humidity(float value)
{
  char buff[8];
  snprintf(buff, sizeof(buff), "%.2f", value);
  publish(HUM, buff);
}

void MQTTManager::publish_aqi(int value)
{
  char buff[8];
  snprintf(buff, sizeof(buff), "%d", value);
  publish(AQI, buff);
}

void MQTTManager::publish_soil(float value)
{
  char buff[8];
  snprintf(buff, sizeof(buff), "%.2f", value);
  publish(SOIL, buff);
}

void MQTTManager::publish_gas(float value)
{
  char buff[8];
  snprintf(buff, sizeof(buff), "%.2f", value);
  publish(GAS, buff);
}

void MQTTManager::publish(const char* topic, const char* value)
{
  char full_topic[256];
  snprintf(full_topic, sizeof(full_topic), "%s/%s/%s", MAIN_MQTT_TOPIC, device_id, topic);
  Serial.println(full_topic);
  client->publish(full_topic, value);
}

void MQTTManager::publish_sensors(float temp, float hum, float soil, float gas, int aqi)
{
  Serial.println("Publish sensors MQTT");
  publish_temperature(temp);
  publish_humidity(hum);
  publish_soil(soil);
  publish_gas(gas);
  publish_aqi(aqi);
  
}

bool MQTTManager::is_connected()
{
  return client->connected();
}