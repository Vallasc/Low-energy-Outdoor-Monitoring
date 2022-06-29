#include "mqttmanager.h"

void MQTTManager::connect()
{
    client->setServer(SERVER, 1883);
    while (!client->connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client->connect(DEVICE_ID, "admin", "admin")) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client->state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void MQTTManager::publish_temperature(float value)
{
    publish(TEMP, String(value));
}

void MQTTManager::publish_humidity(float value)
{
    publish(HUM, String(value));
}

void MQTTManager::publish(String topic, String value) 
{
    client->publish((char * ) topic.c_str(), (char * ) value.c_str());
}

bool MQTTManager::is_connected()
{
    return client->connected();
}