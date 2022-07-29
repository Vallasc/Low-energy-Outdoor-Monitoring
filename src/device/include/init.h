#ifndef INIT_H
#define INIT_H

#include <WebServer.h>
#include <ArduinoJson.h>
#include "state.h"
#include "wifi_utils.h"

#define JSON_ID "id"
#define JSON_NAME "name"
#define JSON_PROTOCOL "protocol"
#define JSON_SAMPLE_FREQUENCY "sampleFrequency"
#define JSON_CONFIG_FREQUENCY "configUpdateFrequency"
#define JSON_MIN_GAS_VALUE "minGasValue"
#define JSON_MAX_GAS_VALUE "maxGasValue"
#define JSON_PROXY_PORT "proxyPort"
#define JSON_MQTT_PORT "mqttPort"
#define JSON_CONFIG_PORT "udpPort"
#define JSON_HOST "host"
#define JSON_TOKEN "token"
#define JSON_WIFI_SSID "wifiSsid"
#define JSON_WIFI_PASS "wifiPassword"
#define JSON_LAT "latitude"
#define JSON_LON "longitude"
#define JSON_PERFORMANCE "enablePerformanceMonitoring"

class InitServer
{
  public:
    const char *AP_SSID = "LOMO-DEVICE";
    InitServer() {}
    ~InitServer() {}
    void init();
    void stop();
    void handle_client();
    void handle_options();
    bool is_inited()
    {
      return init_done;
    }

    void init_from_json(String json_obj);

  private:
    WebServer *server;
    bool init_done = false;
    void handle_post_init();
};

#endif