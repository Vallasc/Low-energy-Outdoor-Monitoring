#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H


#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "protocolmanager.h"
#include "config.h"


class HTTPManager : ProtocolManager{
    public:
        HTTPClient* client;
        HTTPManager(WiFiClient wifiClient){
            client = new HTTPClient();
            client->begin(wifiClient, "http://"+ String(SERVER) + "/reading");
        }

        ~HTTPManager(){
            delete(client);
        }

        void publish(float temp, float hum);


};


#endif