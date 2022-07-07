#include "coap_manager.h"

void CoAPManager::publish_sensors(float temp, float hum, float soil, float aqi)
{
    char payload[256];
    snprintf(payload, sizeof(payload), "%s/%.2f/%.2f/%.2f/%.2f", token, temp, hum, soil, aqi);
    Serial.println(payload);
    client->put(address, port, COAP_URL, payload);
}

void CoAPManager::get_config()
{

}

void CoAPManager::callback_response_config(CoapPacket &packet, IPAddress ip, int port)
{

}
