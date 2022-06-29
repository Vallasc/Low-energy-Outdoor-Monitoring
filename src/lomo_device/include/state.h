#ifndef STATE_H
#define STATE_H

#include <Preferences.h>

#define DEVICE_CONFIGURED "CONF" // bool
#define DEVICE_ID "ID" // String
#define DEVICE_NAME "NAME" // String
#define DEVICE_TOKEN "TOKEN" // String
#define MIN_GAS_VALUE "MIN_GAS" // float
#define MAX_GAS_VALUE "MAX_GAS" // flaot
#define PROTOCOL_TYPE "PROTOCOL" // String HTTP, MQTT 
#define PROXY_PORT "PROXY" // Int
#define MQTT_PORT "MQTT" // Int
#define TOKEN "TOKEN" // String
#define HOST "HOST" // String
#define SAMPLE_FREQUENCY "S_FREQ" // Int sec
#define WIFI_SSID "W_SSID" // String
#define WIFI_PASS "W_PASS" // String

#define PREF_NAME "LOMO"

namespace 
{
  Preferences preferences;
  Preferences* open_preferences()
  {
    preferences.begin(PREF_NAME, false);
    return &preferences;
  }

  void close_preferences()
  {
    preferences.end();
  }

  void print_config()
  {
    open_preferences();
    Serial.println("- Device configured: " + String(preferences.getBool(DEVICE_CONFIGURED, false)));
    Serial.println("- id: " + preferences.getString(DEVICE_ID, "NONE"));
    Serial.println("- name: " + preferences.getString(DEVICE_NAME, "NONE"));
    Serial.println("- protocol: " + preferences.getString(PROTOCOL_TYPE, "NONE"));
    Serial.println("- sampleFrequency: " + String(preferences.getInt(SAMPLE_FREQUENCY, -1)));
    Serial.println("- minGasValue: " + String(preferences.getInt(MIN_GAS_VALUE, -1)));
    Serial.println("- maxGasValue: " + String(preferences.getInt(MAX_GAS_VALUE, -1)));
    Serial.println("- proxyPort: " + String( preferences.getInt(PROXY_PORT, -1)));
    Serial.println("- mqttPort: " + String(preferences.getInt(MQTT_PORT, -1)));
    Serial.println("- host: " + preferences.getString(HOST, "NONE"));
    Serial.println("- token: " + preferences.getString(TOKEN, "NONE"));
    Serial.println("- wfiSsid: " + preferences.getString(WIFI_SSID, "NONE"));
    Serial.println("- wifiPassword: " + preferences.getString(WIFI_PASS, "NONE"));
    close_preferences();
  }
}

#endif