#include "init.h"

void InitAPWebServer::init()
{
  // Set static IP
  IPAddress AP_LOCAL_IP(192, 168, 1, 10);
  IPAddress AP_GATEWAY_IP(192, 168, 1, 254);
  IPAddress AP_NETWORK_MASK(255, 255, 255, 0);
  if (!WiFi.softAPConfig(AP_LOCAL_IP, AP_GATEWAY_IP, AP_NETWORK_MASK)) {
    Serial.println("AP Config Failed");
    return;
  }
  IPAddress IP = WiFi.softAPIP();
  Serial.println("Static IP: " + AP_LOCAL_IP.toString());

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

    open_preferences();
    if(obj.containsKey(JSON_ID)){
      String id = obj[JSON_ID];
      preferences.putString(DEVICE_ID, id);
    }
    if(obj.containsKey(JSON_NAME)){
      String name = obj[JSON_NAME];
      preferences.putString(DEVICE_NAME, name);
    }
    if(obj.containsKey(JSON_PROTOCOL)){
      String protocol = obj[JSON_PROTOCOL];
      preferences.putString(PROTOCOL_TYPE, protocol);
    }
    if(obj.containsKey(JSON_SAMPLE_FREQUENCY)){
      int sampleFrequency = obj[JSON_SAMPLE_FREQUENCY];
      preferences.putInt(SAMPLE_FREQUENCY, sampleFrequency);
    }
    if(obj.containsKey(JSON_MIN_GAS_VALUE)){
      int minGasValue = obj[JSON_MIN_GAS_VALUE];
      preferences.putInt(MIN_GAS_VALUE, minGasValue);
    }
    if(obj.containsKey(JSON_MAX_GAS_VALUE)){
      int maxGasValue = obj[JSON_MAX_GAS_VALUE];
      preferences.putInt(MAX_GAS_VALUE, maxGasValue);
    }
    if(obj.containsKey(JSON_PROXY_PORT)){
      int proxyPort = obj[JSON_PROXY_PORT];
      preferences.putInt(PROXY_PORT, proxyPort);
    }
    if(obj.containsKey(JSON_MQTT_PORT)){
      int mqttPort = obj[JSON_MQTT_PORT];
      preferences.putInt(MQTT_PORT, mqttPort);
    }
    if(obj.containsKey(JSON_HOST)){
      String host = obj[JSON_HOST];
      preferences.putString(HOST, host);
    }
    if(obj.containsKey(JSON_TOKEN)){
      String host = obj[JSON_TOKEN];
      preferences.putString(TOKEN, host);
    }
    if(obj.containsKey(JSON_WIFI_SSID)){
      String wfiSsid = obj[JSON_WIFI_SSID];
      preferences.putString(WIFI_SSID, wfiSsid);
    }
    if(obj.containsKey(JSON_WIFI_PASS)){
      String wifiPassword = obj[JSON_WIFI_PASS];
      preferences.putString(WIFI_PASS, wifiPassword);
    }
    preferences.putBool(DEVICE_CONFIGURED, true);
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