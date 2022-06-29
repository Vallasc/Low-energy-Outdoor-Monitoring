#include "http_manager.h"

void HTTPManager::publish(float temp, float hum){
    String json = "{\"temp\":" + String(temp) + ",\"hum\":" + String(hum) + "}";
}
