#include "iotsmartdevice.h"


void MessageProtocol::wifiConnect() {
    Serial.println();
    Serial.print("Connecting to ");
    open_preferences();
    String ssid = preferences.getString(WIFI_SSID, "NONE");
    String password = preferences.getString(WIFI_PASS, "NONE");
    close_preferences();

    Serial.println(ssid);
    WiFi.begin(ssid.c_str(), password.c_str());
    close_preferences();

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}



