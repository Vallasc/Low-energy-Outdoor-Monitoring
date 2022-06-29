#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <DHT.h>
#include <PubSubClient.h> 
#include <WiFi.h>
#include "protocolmanager.h"
#include "config.h"


#define VALUE "CIAO"

#define HUM "/hum"
#define TEMP "/temp"

class MQTTManager : ProtocolManager{
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

    private:
        void publish(String topic, String value);
        
};

#endif