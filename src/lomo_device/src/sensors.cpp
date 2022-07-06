#include "sensors.h"

float Sensors::get_temperature()
{
  return dht->readTemperature();
}

float Sensors::get_humidity()
{
  return dht->readHumidity();
}

float Sensors::get_soil()
{
  return analogRead(SOIL_PIN);
}

float Sensors::get_gas()
{
  return analogRead(GAS_PIN);
}
