#include "http_manager.h"

void HTTPManager::publish(float temp, float hum){
    String json = "{\"temp\":" + String(temp) + ",\"hum\":" + String(hum) + "}";

    Serial.println(json);

}


void HTTPManager::httpPOSTRequest(const char* serverName, float temp, float hum, int aqi) {
  WiFiClient client;
  HTTPClient http;
    
  

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  
  http.addHeader("Autorization:", "Bearer admin");
  String httpRequestData = "{\"temp\":" + String(temp) + ",\"hum\":" + String(hum) +",\"aqi\":" + String(aqi) + "}";
  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestData);

  
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
        
  // Free resources
  http.end();

  /*if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();*/

  //return payload+json;


}
