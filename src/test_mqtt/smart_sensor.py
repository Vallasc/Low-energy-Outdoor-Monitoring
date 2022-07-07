import time

import paho.mqtt.client as mqtt
from faker import Faker

MQTT_BROKER_URL    = "localhost"

TEMP_TOPIC = "devices/foo/temp"
SOIL_TOPIC = "devices/foo/soil"
HUM_TOPIC = "devices/foo/hum"
AQI_TOPIC = "devices/foo/aqi"

MQTT_USER = "admin"
MQTT_PASS = "admin"

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe("devices/#")

def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))

mqttc = mqtt.Client()
mqttc.username_pw_set(MQTT_USER, MQTT_PASS)
mqttc.connect(MQTT_BROKER_URL)

mqttc.on_connect = on_connect
mqttc.on_message = on_message

fake = Faker()

while True:
    temp = fake.random_int(min=10, max=30)
    soil = fake.random_int(min=0, max=255)
    hum = fake.random_int(min=0, max=100)
    aqi = fake.random_int(min=0, max=10)

    mqttc.loop()
    mqttc.publish(TEMP_TOPIC, str(temp))
    mqttc.publish(SOIL_TOPIC, str(soil))
    mqttc.publish(HUM_TOPIC, str(hum))
    mqttc.publish(AQI_TOPIC, str(aqi))

    time.sleep(1)