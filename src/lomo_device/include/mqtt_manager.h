#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <DHT.h>
#include <PubSubClient.h> 
#include <WiFi.h>
#include "protocol_manager.h"
#include "config.h"

// /devies/foo/temp
#define MAIN_MQTT_TOPIC "devices"
#define HUM "hum"
#define TEMP "temp"
#define SOIL "soil"
#define AQI "aqi"

class MQTTManager : ProtocolManager {
    public:
        PubSubClient* client;

        MQTTManager(WiFiClient* wifiClient, const char* host, int port, const char* device_id, const char* token){
            client = new PubSubClient(*wifiClient);
            client->setServer(host, port);
            this->device_id = device_id;
            this->token = token;
        }
 
        ~MQTTManager(){
             delete(client);
        }

        bool connect();
        bool is_connected();

        void publish_temperature(float value);
        void publish_humidity(float value);
        void publish_soil(float value);
        void publish_aqi(float value);
        void publish(String topic, String value);
    private:
        const char* device_id;
        const char* token;
        const int max_mqtt_retries = 50;
        
};

#endif