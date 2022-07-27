#ifndef PROTOCOLMANAGER_H
#define PROTOCOLMANAGER_H

class ProtocolManager {
  public:
    void publish_sensors(float temp, float hum, float soil, float aqi) {}
    bool begin() {
      return true;
    }
};

#endif