#ifndef STATE_H
#define STATE_H

#include <Preferences.h>
#include "debug_config.h"

#define KEY_DEVICE_CONFIGURED "CONF" // bool
#define KEY_DEVICE_ID "ID" // String
#define KEY_DEVICE_NAME "NAME" // String
#define KEY_MIN_GAS_VALUE "MIN_GAS" // float
#define KEY_MAX_GAS_VALUE "MAX_GAS" // flaot
#define KEY_PROTOCOL_TYPE "PROTOCOL" // String HTTP, MQTT 
#define KEY_PROXY_PORT "PROXY" // Int
#define KEY_MQTT_PORT "MQTT" // Int
#define KEY_TOKEN "TOKEN" // String
#define KEY_HOST "HOST" // String
#define KEY_SAMPLE_FREQUENCY "S_FREQ" // Int sec
#define KEY_WIFI_SSID "W_SSID" // String
#define KEY_WIFI_PASS "W_PASS" // String
#define KEY_LAT "LAT" // double
#define KEY_LONG "LONG" // double

#define PREF_NAME "LOMO"

namespace 
{
  bool DEVICE_CONFIGURED;
  String DEVICE_ID;
  String DEVICE_NAME;
  int MIN_GAS_VALUE;
  int MAX_GAS_VALUE;
  String PROTOCOL_TYPE;
  int PROXY_PORT;
  int MQTT_PORT;
  String TOKEN;
  String HOST;
  int SAMPLE_FREQUENCY;
  String WIFI_SSID;
  String WIFI_PASS;
  double LAT;
  double LONG;
  Preferences preferences;

  static Preferences* open_preferences()
  {
    preferences.begin(PREF_NAME, false);
    return &preferences;
  }

  static void close_preferences()
  {
    preferences.end();
  }

  static void print_config()
  {
    Serial.println("Config:");
    Serial.println("- Device configured: " + String(DEVICE_CONFIGURED));
    Serial.println("- id: " + DEVICE_ID);
    Serial.println("- name: " + DEVICE_NAME);
    Serial.println("- protocol: " + PROTOCOL_TYPE);
    Serial.println("- sampleFrequency: " + String(SAMPLE_FREQUENCY));
    Serial.println("- minGasValue: " + String(MIN_GAS_VALUE));
    Serial.println("- maxGasValue: " + String(MAX_GAS_VALUE));
    Serial.println("- proxyPort: " + String(PROXY_PORT));
    Serial.println("- mqttPort: " + String(MQTT_PORT));
    Serial.println("- host: " + HOST);
    Serial.println("- token: " + TOKEN);
    Serial.println("- wfiSsid: " + WIFI_SSID);
    Serial.println("- wifiPassword: " + WIFI_PASS);
    Serial.println("- latitude: " + String(LAT));
    Serial.println("- longitude: " + String(LONG));
  }

  static void load_config()
  {
    open_preferences();
    DEVICE_CONFIGURED = preferences.getBool(KEY_DEVICE_CONFIGURED, false);
    DEVICE_ID = preferences.getString(KEY_DEVICE_ID, "NONE");
    DEVICE_NAME = preferences.getString(KEY_DEVICE_NAME, "NONE");
    PROTOCOL_TYPE = preferences.getString(KEY_PROTOCOL_TYPE, "NONE");
    SAMPLE_FREQUENCY = preferences.getInt(KEY_SAMPLE_FREQUENCY, -1);
    MIN_GAS_VALUE = preferences.getInt(KEY_MIN_GAS_VALUE, -1);
    MAX_GAS_VALUE = preferences.getInt(KEY_MAX_GAS_VALUE, -1);
    PROXY_PORT = preferences.getInt(KEY_PROXY_PORT, -1);
    MQTT_PORT = preferences.getInt(KEY_MQTT_PORT, -1);
    HOST = preferences.getString(KEY_HOST, "NONE");
    TOKEN = preferences.getString(KEY_TOKEN, "NONE");
    WIFI_SSID = preferences.getString(KEY_WIFI_SSID, "NONE");
    WIFI_PASS = preferences.getString(KEY_WIFI_PASS, "NONE");
    LAT = preferences.getDouble(KEY_LAT, -1);
    LONG = preferences.getDouble(KEY_LONG, -1);
    close_preferences();
  }


  static void load_debug_config()
  {
    open_preferences();
    preferences.putBool(KEY_DEVICE_CONFIGURED, DEBUG_DEVICE_CONFIGURED);
    preferences.putString(KEY_DEVICE_ID, DEBUG_DEVICE_ID);
    preferences.putString(KEY_DEVICE_NAME, DEBUG_DEVICE_NAME);
    preferences.putString(KEY_PROTOCOL_TYPE, DEBUG_PROTOCOL_TYPE);
    preferences.putInt(KEY_SAMPLE_FREQUENCY, DEBUG_SAMPLE_FREQUENCY);
    preferences.putInt(KEY_MIN_GAS_VALUE, DEBUG_MIN_GAS_VALUE);
    preferences.putInt(KEY_MAX_GAS_VALUE, DEBUG_MAX_GAS_VALUE);
    preferences.putInt(KEY_PROXY_PORT, DEBUG_PROXY_PORT);
    preferences.putInt(KEY_MQTT_PORT, DEBUG_MQTT_PORT);
    preferences.putString(KEY_HOST, DEBUG_HOST);
    preferences.putString(KEY_TOKEN, DEBUG_TOKEN);
    preferences.putString(KEY_WIFI_SSID, DEBUG_WIFI_SSID);
    preferences.putString(KEY_WIFI_PASS, DEBUG_WIFI_PASS);
    preferences.putDouble(KEY_LAT, DEBUG_LAT);
    preferences.putDouble(KEY_LONG, DEBUG_LONG);
    close_preferences();
  }
}

#endif