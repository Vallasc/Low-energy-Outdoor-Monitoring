#ifndef PROTOCOLMANAGER_H
#define PROTOCOLMANAGER_H

class ProtocolManager{
public:
   virtual void publish_temperature(float value);
   virtual void publish_humidity(float value);

};

#endif