#include "coap_manager.h"

void CoAPManager::publish_sensors(float temp, float hum, float soil, float gas, int aqi, int wifi_rssi)
{
    char payload[256];
    snprintf(payload, sizeof(payload), "%s/%.2f/%.2f/%.2f/%.2f/%d/%d/%lf,%lf", token, temp, hum, soil, gas, aqi, wifi_rssi, latitude, longitude);
    Serial.println(payload);
    client->put(address, port, COAP_URL, payload);
}

void CoAPManager::get_config()
{

}

void CoAPManager::callback_response_config(CoapPacket &packet, IPAddress ip, int port)
{

}
