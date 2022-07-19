#ifndef SENSORS_H
#define SENSORS_H

#include <DHT.h>
#include "config.h"

#define SOIL_LOW 3000
#define SOIL_HIGH 1100


class Sensors {
  public:
    Sensors(float min_gas_value, float max_gas_value)
    {
      dht = new DHT(DHT_PIN, DHT_TYPE);
      dht->begin();
      pinMode(GAS_PIN, INPUT);
      pinMode(SOIL_PIN, INPUT);

      this->min_gas_value = min_gas_value;
      this->max_gas_value = max_gas_value;

      for(int i=0; i<5; i++)
        v[i] = -1;
    }

    ~Sensors()
    {
      delete(dht);
    }

    // Temperature Celsius
    float get_temperature();
    // Humidity %
    float get_humidity();
    // Wet %
    float get_soil();
    //AQI
    float get_gas();

    int get_aqi();

    // MQ135 	10～1000ppm( ammonia gas, toluene, Hydrogen, smoke)
    // Atmospheric CO2 Level:  400ppm
    // Average indoor CO2:     350-450ppm
    // Maximum acceptable CO2: 1000ppm
    // Dangerous CO2 levels:   2000ppm
    
  private:
    DHT* dht;
    int aqi_index = 0;
    float min_gas_value;
    float max_gas_value;
    float v[5];
    int aqi = 2;

};

#endif