#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H


#include <HTTPClient.h>
#include <ArduinoJson.h>
//#include "protocol_manager.h"
#include "config.h"
#include "state.h"


class HTTPManager{
    public:
        HTTPClient* client;
        HTTPManager(WiFiClient wifiClient){
            client = new HTTPClient();
            /*open_preferences();
            client->begin(wifiClient, "http://"+ preferences.getString(HOST, "192.168.1.3") + "/reading");
            close_preferences();*/
        }

        ~HTTPManager(){
            delete(client);
        }

        void publish(float temp, float hum);

   void httpPOSTRequest(const char* serverName, float temp, float hum, int aqi);
};


#endif