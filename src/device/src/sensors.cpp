#include "sensors.h"

void Sensors::begin(){
  pinMode(ENABLE_SENSORS_PIN, OUTPUT);
  Serial.println("Sensors enabled");
  pinMode(GAS_PIN, INPUT);
  pinMode(SOIL_PIN, INPUT);
  dht = new DHT(DHT_PIN, DHT_TYPE);
  start();
}

void Sensors::start(){
  dht->begin();
  digitalWrite(ENABLE_SENSORS_PIN, HIGH);
  delay(5000);
}

void Sensors::stop(){
  digitalWrite(ENABLE_SENSORS_PIN, LOW);
}

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
  float reading = analogRead(GAS_PIN);
  reading = reading < GAS_LOW ? GAS_LOW : reading;
  reading = reading > GAS_HIGH ? GAS_HIGH : reading;
  return map(reading, GAS_LOW, GAS_HIGH, 0, 100);
}


int Sensors::get_aqi()
{
  v[*aqi_index] = get_gas();
  *aqi_index = (*aqi_index + 1) % 5;

  if(v[4] == -1.0)
    return aqi;

  int sum = v[0] + v[1]+ v[2] + v[3] + v[4];
  int avg = sum/(5);

  if(avg >= max_gas_value)
    aqi = 0;
  else if(min_gas_value <= avg && avg < max_gas_value)
    aqi = 1;
  else
    aqi = 2;
 return aqi;
}

  
