#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H

#include <Arduino.h>
#include <WiFi.h>

namespace
{
  const int max_wifi_retries = 100;

  bool wifi_connect(const char *ssid, const char *password)
  {
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    int i = max_wifi_retries;
    while (WiFi.status() != WL_CONNECTED && i-- > 0)
    {
      delay(500);
      Serial.print(".");
    }
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.print("WiFi not connected: max wifi retries reached.");
      return false;
    }
    Serial.println();
    Serial.print("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  }

  bool wifi_create_AP()
  {
    // Set static IP
    IPAddress AP_LOCAL_IP(192, 168, 1, 10);
    IPAddress AP_GATEWAY_IP(192, 168, 1, 254);
    IPAddress AP_NETWORK_MASK(255, 255, 255, 0);
    Serial.println("Creating access point");
    if (!WiFi.softAPConfig(AP_LOCAL_IP, AP_GATEWAY_IP, AP_NETWORK_MASK))
    {
      Serial.println("AP Config Failed");
      return false;
    }
    IPAddress IP = WiFi.softAPIP();
    Serial.println("Static IP: " + AP_LOCAL_IP.toString());
    Serial.println("Gateway IP: " + AP_GATEWAY_IP.toString());
    return true;
  }

  // Take measurements of the Wi-Fi strength and return the average result.
  int get_wifi_strength(int points)
  {
    long rssi = 0;
    long averageRSSI = 0;

    for (int i = 0; i < points; i++)
    {
      rssi += WiFi.RSSI();
      delay(20);
    }

    averageRSSI = rssi / points;
    return averageRSSI;
  }
}

#endif