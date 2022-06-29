#include <Arduino.h>
#include <Esp.h>
#include <DHT.h>

#include "mqtt_manager.h"
#include "init.h"
#include "config.h"
#include "state.h"

void wifi_connect()
{
  open_preferences();
  String ssid = preferences.getString(WIFI_SSID, "NONE");
  String password = preferences.getString(WIFI_PASS, "NONE");
  close_preferences();
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid.c_str(), password.c_str());

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

int sampleFrequency = 10;

void setup() {
  delay(2000);
  Serial.begin(9600);
  Serial.println("LOMO v1.0");

  open_preferences();
  //preferences.clear();
  //preferences.putBool(DEVICE_CONFIGURED, false);
  bool device_configured = preferences.getBool(DEVICE_CONFIGURED, false);
  String protocol = preferences.getString(PROTOCOL_TYPE, "");
  sampleFrequency = preferences.getInt(SAMPLE_FREQUENCY, 1);
  close_preferences();

  if(!device_configured)
  {
    Serial.println("Waiting for initialization...");
    InitAPWebServer init_tasks;
    init_tasks.init();
    while(!init_tasks.is_inited()){
      init_tasks.handle_client();
      delay(500);
    }
    Serial.println("Device configured");
    print_config();
    delay(2000);
    ESP.restart();
  }

  // pinMode(21, OUTPUT);
  // digitalWrite(21, HIGH);
  
  // pinMode(22, OUTPUT);
  // digitalWrite(22, HIGH);

  print_config();
  DHT dht(DHTPIN, DHTTYPE);
  WiFiClient wifiClient;
  MQTTManager mqtt_mng(wifiClient);

  dht.begin();
  wifi_connect();

  if (!mqtt_mng.is_connected())
  {
    mqtt_mng.connect();
  }

  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  mqtt_mng.publish_temperature(temp);
  mqtt_mng.publish_humidity(hum);
}

void loop() {
  Serial.println("CIAO");
  delay(sampleFrequency * 1000);
}