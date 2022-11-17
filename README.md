# Low-energy-Outdoor-Monitoring
The main goal of this project is to implement an outdoor IoT system that collects environmental data, such as soil moisture, temperature, air humidity and air quality.
A special focus was given on reducing the power consumption of devices, optimizing their duration through batteries by way of a custom designed board and *ad hoc* code.
The system is usable by multiple users, each one can sign-up and create an account that can be used to add new devices. Moreover, devices' parameters can be managed and data can be shown on the web interface.
Devices, that runs on *ESP32* SoC, collect environmental data through sensors such as humidity and temperature using *DHT11*, air quality using *MQ135* and soil moisture using a soil capacity sensor.
In addition, user can choose whether to apply machine learning algorithms to the data to predict possible changes in the sensors values.
