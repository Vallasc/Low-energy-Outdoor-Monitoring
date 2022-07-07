#ifndef SENSORS_H
#define SENSORS_H

#include <DHT.h>
#include "config.h"

#define SOIL_LOW 3000
#define SOIL_HIGH 1100

class Sensors {
  public:
    Sensors()
    {
      dht = new DHT(DHT_PIN, DHT_TYPE);
      dht->begin();
      pinMode(GAS_PIN, INPUT);
      pinMode(SOIL_PIN, INPUT);
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
    
    // MQ135 	10～1000ppm( ammonia gas, toluene, Hydrogen, smoke)
    // Atmospheric CO2 Level:  400ppm
    // Average indoor CO2:     350-450ppm
    // Maximum acceptable CO2: 1000ppm
    // Dangerous CO2 levels:   2000ppm
    float get_gas();
  private:
    DHT* dht;
};

#endif