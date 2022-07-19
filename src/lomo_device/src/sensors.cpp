#include "sensors.h"
#include "state.h"

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
  // TODO
  // int v1;
  // Serial.println("GAS:");
  // v1 = rand() % 100;
  // Serial.println(v1);
  
  return analogRead(GAS_PIN);
  //return v1;
}


int Sensors::get_aqi(){    
  Serial.println("Valore i");
  Serial.println(aqi_index);

  v[aqi_index] = get_gas();
  
  Serial.println("Valore v[i]:");
  Serial.println(v[aqi_index]);

  aqi_index = (aqi_index + 1) % 5;

  if(v[4] == -1.0)
    return aqi;

  int somma = v[0] + v[1]+ v[2] + v[3] + v[4];
  Serial.println("Somma:");
  Serial.println(somma);
  int media = somma/(5);
  Serial.println("Media:");
  Serial.println(media);

  if(media >= max_gas_value)
    aqi = 0;
  if(min_gas_value <= media && media < max_gas_value)
    aqi = 1;
  else
    aqi = 2;

  Serial.println("AQI:");
  Serial.println(aqi);

 return aqi;
 }

  
