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
  float reading = analogRead(SOIL_PIN);
  reading = reading > SOIL_LOW ? SOIL_LOW : reading;
  reading = reading < SOIL_HIGH ? SOIL_HIGH : reading;
  return map(reading, SOIL_LOW, SOIL_HIGH, 0, 100);
}

float Sensors::get_gas()
{
  return analogRead(GAS_PIN);
}
