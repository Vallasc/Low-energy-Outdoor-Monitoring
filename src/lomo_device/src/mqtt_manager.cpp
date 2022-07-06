#include "mqtt_manager.h"

void MQTTManager::connect()
{
    open_preferences();
    String server = preferences.getString(HOST, "NONE");
    int mqtt_port = preferences.getInt(MQTT_PORT, 1883);
    String device_id = preferences.getString(DEVICE_ID, "NONE");
    String token = preferences.getString(TOKEN, "NONE");
    close_preferences();

    client->setServer(server.c_str(), mqtt_port);
    while (!client->connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client->connect(device_id.c_str(), device_id.c_str(), token.c_str())) {
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

void MQTTManager::publish_aqi(float value)
{
    publish(AQI, String(value));
}

void MQTTManager::publish(String topic, String value) 
{
    client->publish((char * ) topic.c_str(), (char * ) value.c_str());
}

bool MQTTManager::is_connected()
{
    return client->connected();
}