#ifndef IOTSMARTDEVICE_H
#define IOTSMARTDEVICE_H

#include <DHT.h>
#include <PubSubClient.h> 
#include <WiFi.h>
#include "config.h"
#include "state.h"

class MessageProtocol{
    public:
        //PubSubClient* client;

        MessageProtocol(WiFiClient wifiClient){
            //client = new PubSubClient(wifiClient);
        }

        ~MessageProtocol(){
            //delete(client);
        }

        void wifiConnect();
        /*void mqttReConnect();
        void mqttEmit(String topic, String value);*/
};

#endif