#include "init.h"

void InitServer::init()
{
  wifi_create_AP();
  // Setup web server connection
  this->server = new WebServer(80);
  this->server->enableCORS(true);
  this->server->begin();
  this->server->on("/init", HTTP_OPTIONS, std::bind(&InitServer::handle_options, this));
  this->server->on("/", HTTP_GET, std::bind(&InitServer::handle_get, this));
  this->server->on("/init", HTTP_POST, std::bind(&InitServer::handle_post_init, this));

  Serial.println("WebServer setup: DONE");
}

void InitServer::handle_options()
{
  server->send(200, "application/json", "");
}

void InitServer::handle_client()
{
  this->server->handleClient();
}

void InitServer::handle_get()
{
  Serial.println("Handle GET /");
  char* html = "<html> \
                <body> \
                <h1>Device configuration</h1> \
                <form action=\"/init\" method=\"post\" target=\"_top\"> \
                Json config<br> \
                <input type=\"text\" name=\"json\" required><br><br> \
                <input type=\"submit\" value=\"Save\"> \
                </form> \
                </body> \
                </html>";
  server->send(200, "text/html", html);
}

void InitServer::handle_post_init()
{
  Serial.println("Handle init");
  if (server->hasArg("json")) {
    String plain = server->arg("json");
    init_from_json(plain);
    init_done = true;
  }
    char* html = "<html> \
                <body> \
                <h1>Device initialized</h1> \
                </body> \
                </html>";
  server->send(200, "text/html", html);
  stop();
}

void InitServer::init_from_json(String json_obj)
{
  DynamicJsonDocument doc(2048);
  deserializeJson(doc, json_obj);
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
  if(obj.containsKey(JSON_CONFIG_FREQUENCY)){
    int configFrequency = obj[JSON_CONFIG_FREQUENCY];
    pref->putInt(KEY_CONFIG_FREQUENCY, configFrequency);
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
  if(obj.containsKey(JSON_CONFIG_PORT)){
    int configPort = obj[JSON_CONFIG_PORT];
    pref->putInt(KEY_CONFIG_PORT, configPort);
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
  if(obj.containsKey(JSON_LAT)){
    double lat = obj[JSON_LAT];
    pref->putDouble(KEY_LAT, lat);
  }
  if(obj.containsKey(JSON_LON)){
    double lon = obj[JSON_LON];
    pref->putDouble(KEY_LON, lon);
  }
  if(obj.containsKey(JSON_PERFORMANCE)){
    bool perf = obj[JSON_PERFORMANCE];
    pref->putBool(KEY_PERF, perf);
  }
  preferences.putBool(KEY_DEVICE_CONFIGURED, true);
  close_preferences();
}

void InitServer::stop()
{
  server->close();
  delete(server);
}