#ifndef STATE_H
#define STATE_H

#include <Preferences.h>

#define KEY_DEVICE_CONFIGURED "CONF" // bool
#define KEY_DEVICE_ID "ID" // String
#define KEY_DEVICE_NAME "NAME" // String
#define KEY_MIN_GAS_VALUE "MIN_GAS" // float
#define KEY_MAX_GAS_VALUE "MAX_GAS" // flaot
#define KEY_PROTOCOL_TYPE "PROTOCOL" // String HTTP, MQTT 
#define KEY_PROXY_PORT "PROXY" // Int
#define KEY_CONFIG_PORT "CONF_P" // Int
#define KEY_MQTT_PORT "MQTT" // Int
#define KEY_TOKEN "TOKEN" // String
#define KEY_HOST "HOST" // String
#define KEY_SAMPLE_FREQUENCY "S_FREQ" // Int sec
#define KEY_CONFIG_FREQUENCY "C_FREQ" // Int sec
#define KEY_WIFI_SSID "W_SSID" // String
#define KEY_WIFI_PASS "W_PASS" // String
#define KEY_LAT "LAT" // double
#define KEY_LON "LON" // double
#define KEY_PERF "PERF" // bool

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
  int CONFIG_PORT;
  String TOKEN;
  String HOST;
  int SAMPLE_FREQUENCY;
  int CONFIG_FREQUENCY;
  String WIFI_SSID;
  String WIFI_PASS;
  double LAT;
  double LON;
  bool PERFORMANCE_MONITORING;
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
    Serial.println("- device configured: " + String(DEVICE_CONFIGURED));
    Serial.println("- id: " + DEVICE_ID);
    Serial.println("- name: " + DEVICE_NAME);
    Serial.println("- protocol: " + PROTOCOL_TYPE);
    Serial.println("- sampleFrequency: " + String(SAMPLE_FREQUENCY));
    Serial.println("- configFrequency: " + String(CONFIG_FREQUENCY));
    Serial.println("- minGasValue: " + String(MIN_GAS_VALUE));
    Serial.println("- maxGasValue: " + String(MAX_GAS_VALUE));
    Serial.println("- proxyPort: " + String(PROXY_PORT));
    Serial.println("- mqttPort: " + String(MQTT_PORT));
    Serial.println("- configPort: " + String(CONFIG_PORT));
    Serial.println("- host: " + HOST);
    Serial.println("- token: " + TOKEN);
    Serial.println("- wfiSsid: " + WIFI_SSID);
    Serial.println("- wifiPassword: " + WIFI_PASS);
    Serial.println("- latitude: " + String(LAT));
    Serial.println("- longitude: " + String(LON));
    Serial.println("- performance monitoring: " + String(PERFORMANCE_MONITORING));
  }

  static void load_config()
  {
    open_preferences();
    DEVICE_CONFIGURED = preferences.getBool(KEY_DEVICE_CONFIGURED, false);
    DEVICE_ID = preferences.getString(KEY_DEVICE_ID, "NONE");
    DEVICE_NAME = preferences.getString(KEY_DEVICE_NAME, "NONE");
    PROTOCOL_TYPE = preferences.getString(KEY_PROTOCOL_TYPE, "NONE");
    SAMPLE_FREQUENCY = preferences.getInt(KEY_SAMPLE_FREQUENCY, -1);
    CONFIG_FREQUENCY = preferences.getInt(KEY_CONFIG_FREQUENCY, -1);
    MIN_GAS_VALUE = preferences.getInt(KEY_MIN_GAS_VALUE, -1);
    MAX_GAS_VALUE = preferences.getInt(KEY_MAX_GAS_VALUE, -1);
    PROXY_PORT = preferences.getInt(KEY_PROXY_PORT, -1);
    MQTT_PORT = preferences.getInt(KEY_MQTT_PORT, -1);
    CONFIG_PORT = preferences.getInt(KEY_CONFIG_PORT, -1);
    HOST = preferences.getString(KEY_HOST, "NONE");
    TOKEN = preferences.getString(KEY_TOKEN, "NONE");
    WIFI_SSID = preferences.getString(KEY_WIFI_SSID, "NONE");
    WIFI_PASS = preferences.getString(KEY_WIFI_PASS, "NONE");
    LAT = preferences.getDouble(KEY_LAT, -1);
    LON = preferences.getDouble(KEY_LON, -1);
    PERFORMANCE_MONITORING = preferences.getBool(KEY_PERF, false);
    close_preferences();
  }

  static void reset_pref()
  {
    open_preferences();
    preferences.putBool(KEY_DEVICE_CONFIGURED, false);
    close_preferences();
  }
}

#endif