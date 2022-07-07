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

    float get_gas();
  private:
    DHT* dht;
};

#endif