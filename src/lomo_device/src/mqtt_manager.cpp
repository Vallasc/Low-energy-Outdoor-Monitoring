#include "mqtt_manager.h"

bool MQTTManager::connect()
{
    int i = this->max_mqtt_retries;
    while (!client->connected() && i-- > 0) {
        Serial.print("Connecting to MQTT broker ");
        if (client->connect(device_id, device_id, token)) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client->state());
            delay(5000);
        }
    }
    if(!client->connected() ){
        Serial.print("Broker not connected: max mqtt retries reached.");
        return false;
    }
    return true;
}

void MQTTManager::publish_temperature(float value)
{
    publish(TEMP, String(value));
}

void MQTTManager::publish_humidity(float value)
{
    publish(HUM, String(value));
}

void MQTTManager::publish_aqi(float value)
{
    publish(AQI, String(value));
}

void MQTTManager::publish_soil(float value)
{
    publish(SOIL, String(value));
}

void MQTTManager::publish(String topic, String value) 
{
    String full_topic = String(MAIN_MQTT_TOPIC) + "/" + String(device_id) + "/" + topic;
    Serial.println(full_topic);
    client->publish((char * ) full_topic.c_str(), (char * ) value.c_str());
}

bool MQTTManager::is_connected()
{
    return client->connected();
}