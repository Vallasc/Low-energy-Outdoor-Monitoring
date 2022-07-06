#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <DHT.h>
#include <PubSubClient.h> 
#include <WiFi.h>
//#include "protocol_manager.h"
#include "config.h"
#include "state.h"

#define VALUE "CIAO"

#define HUM "/hum"
#define TEMP "/temp"
#define AQI "/aqi"

class MQTTManager {
    public:
        PubSubClient* client;

        MQTTManager(WiFiClient wifiClient){
            client = new PubSubClient(wifiClient);
        }

        ~MQTTManager(){
            delete(client);
        }

        void connect();
        bool is_connected();
        void publish_temperature(float value);
        void publish_humidity(float value);
        void publish_aqi(float value);

    private:
        void publish(String topic, String value);
        
};

#endif