#ifndef SENSORS_H
#define SENSORS_H

#include <DHT.h>
#include "config.h"

#define SOIL_LOW 3000
#define SOIL_HIGH 1100

#define GAS_LOW 0
#define GAS_HIGH 2000

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

    void begin();
    void start();
    void stop();
    // Temperature Celsius
    float get_temperature();
    // Humidity %
    float get_humidity();
    // Wet %
    float get_soil();
    //AQI
    float get_gas();
    int get_aqi();
    
  private:
    DHT* dht;
    int aqi_index = 0;
    float min_gas_value;
    float max_gas_value;
    float v[5];
    int aqi = 2;

};

#endif