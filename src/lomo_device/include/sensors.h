#ifndef SENSORS_H
#define SENSORS_H

#include <DHT.h>
#include "config.h"

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

    float get_temperature();
    float get_humidity();
    float get_soil();
    float get_gas();

  private:
    DHT* dht;
};

#endif