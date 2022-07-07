#ifndef PROTOCOLMANAGER_H
#define PROTOCOLMANAGER_H

class ProtocolManager {
  public:
    void publish_temperature(float value);
    void publish_humidity(float value);
    void publish_soil(float value);
    void publish_aqi(float value);
};

#endif