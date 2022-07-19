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
#define GAS "gas"
#define OUT "outdoor"

class MQTTManager : public ProtocolManager {
    public:
        PubSubClient* client;

        MQTTManager(WiFiClient* wifiClient, const char* _host, int port, const char* _device_id, const char* _token){
            client = new PubSubClient(*wifiClient);
            strcpy(host, _host);
            client->setServer(host, port);
            strcpy(device_id, _device_id);
            strcpy(token, _token);
        }
 
        ~MQTTManager(){
             delete(client);
        }

        bool begin();
        bool is_connected();

        void publish_temperature(float value);
        void publish_humidity(float value);
        void publish_soil(float value);
        void publish_aqi(int value);
        void publish_gas(float value);
        void publish_outdoor(float value);
        void publish(const char* topic, const char* value);
        void publish_sensors(float temp, float hum, float soil, float gas, int aqi);
    private:
        char host[32];
        char device_id[64];
        char token[64];
        const int max_mqtt_retries = 50;
        
};

#endif