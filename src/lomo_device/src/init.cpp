#include "init.h"

void InitAPWebServer::init()
{
  
  wifi_create_AP();
  // Setup web server connection
  this->server = new WebServer(80);
  this->server->enableCORS(true);
  this->server->begin();
  this->server->on("/init", HTTP_OPTIONS, std::bind(&InitAPWebServer::handle_options, this));
  this->server->on("/init", HTTP_POST, std::bind(&InitAPWebServer::handle_post_init, this));

  Serial.println("WebServer setup: DONE");
}

void InitAPWebServer::handle_options()
{
  server->send(200, "application/json", "");
}

void InitAPWebServer::handle_client()
{
  this->server->handleClient();
}

void InitAPWebServer::handle_post_init()
{
  Serial.println("Handle init");
  if (server->hasArg("plain")) {
    String plain = server->arg("plain");
    DynamicJsonDocument doc(2000);
    deserializeJson(doc, plain);
    JsonObject obj = doc.as<JsonObject>();

    Preferences* pref = open_preferences();
    if(obj.containsKey(JSON_ID)){
      String id = obj[JSON_ID];
      pref->putString(KEY_DEVICE_ID, id);
    }
    if(obj.containsKey(JSON_NAME)){
      String name = obj[JSON_NAME];
      pref->putString(KEY_DEVICE_NAME, name);
    }
    if(obj.containsKey(JSON_PROTOCOL)){
      String protocol = obj[JSON_PROTOCOL];
      pref->putString(KEY_PROTOCOL_TYPE, protocol);
    }
    if(obj.containsKey(JSON_SAMPLE_FREQUENCY)){
      int sampleFrequency = obj[JSON_SAMPLE_FREQUENCY];
      pref->putInt(KEY_SAMPLE_FREQUENCY, sampleFrequency);
    }
    if(obj.containsKey(JSON_MIN_GAS_VALUE)){
      int minGasValue = obj[JSON_MIN_GAS_VALUE];
      pref->putInt(KEY_MIN_GAS_VALUE, minGasValue);
    }
    if(obj.containsKey(JSON_MAX_GAS_VALUE)){
      int maxGasValue = obj[JSON_MAX_GAS_VALUE];
      pref->putInt(KEY_MAX_GAS_VALUE, maxGasValue);
    }
    if(obj.containsKey(JSON_PROXY_PORT)){
      int proxyPort = obj[JSON_PROXY_PORT];
      pref->putInt(KEY_PROXY_PORT, proxyPort);
    }
    if(obj.containsKey(JSON_MQTT_PORT)){
      int mqttPort = obj[JSON_MQTT_PORT];
      pref->putInt(KEY_MQTT_PORT, mqttPort);
    }
    if(obj.containsKey(JSON_HOST)){
      String host = obj[JSON_HOST];
      pref->putString(KEY_HOST, host);
    }
    if(obj.containsKey(JSON_TOKEN)){
      String host = obj[JSON_TOKEN];
      pref->putString(KEY_TOKEN, host);
    }
    if(obj.containsKey(JSON_WIFI_SSID)){
      String wfiSsid = obj[JSON_WIFI_SSID];
      pref->putString(KEY_WIFI_SSID, wfiSsid);
    }
    if(obj.containsKey(JSON_WIFI_PASS)){
      String wifiPassword = obj[JSON_WIFI_PASS];
      pref->putString(KEY_WIFI_PASS, wifiPassword);
    }
    preferences.putBool(KEY_DEVICE_CONFIGURED, true);
    close_preferences();
  }

  init_done = true;
  server->send(200, "application/json", "");
}

void InitAPWebServer::stop()
{
  server->close();
  delete(server);
}