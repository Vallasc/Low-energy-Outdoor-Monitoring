#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H


#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "protocol_manager.h"
#include "config.h"
#include "state.h"


class HTTPManager : ProtocolManager{
    public:
        HTTPClient* client;
        HTTPManager(WiFiClient wifiClient){
            client = new HTTPClient();
            open_preferences();
            client->begin(wifiClient, "http://"+ preferences.getString(HOST, "NONE") + "/reading");
            close_preferences();
        }

        ~HTTPManager(){
            delete(client);
        }

        void publish(float temp, float hum);


};


#endif