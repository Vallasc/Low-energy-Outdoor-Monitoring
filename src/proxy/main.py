import os
import paho.mqtt.client as mqtt
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS

import logging

INFLUXDB_HOST = os.getenv("INFLUX_HOST", "localhost")
INFLUXDB_PORT = os.getenv("INFLUXDB_PORT", "8086")
INFLUXDB_ORG = os.getenv("INFLUXDB_ORG", "lomo")
INFLUXDB_BUCKET = os.getenv("INFLUXDB_BUCKET", "lomo")
INFLUXDB_TOKEN = os.getenv("INFLUX_TOKEN", "adminadminadmin")

MQTT_HOST = os.getenv("MQTT_HOST", "localhost")
MQTT_PORT = os.getenv("MQTT_PORT", "1883")
MQTT_USER = os.getenv("MQTT_USER", "admin")
MQTT_PASS = os.getenv("MQTT_PASS", "admin")

def persists(device_id, topic, value):
    print("persist")
    p = Point("devices").tag("id", device_id).field(topic, float(value))
    write_api.write(bucket=INFLUXDB_BUCKET, org=INFLUXDB_ORG, record=p)
    logging.info(p)

logging.basicConfig(level=logging.INFO)

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe("devices/#")

def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))
    splitted = msg.topic.split("/")
    topic = splitted[-1]
    device_id = splitted[-2]
    if topic == "temp" or topic == "hum" or topic == "aqi" or topic == "soil":
        persists(device_id, topic, msg.payload)
    
influx_client = InfluxDBClient(
   url= "http://" + INFLUXDB_HOST + ":" + INFLUXDB_PORT,
   token=INFLUXDB_TOKEN,
   org=INFLUXDB_ORG
)
write_api = influx_client.write_api(write_options=SYNCHRONOUS)

client = mqtt.Client()
client.username_pw_set(MQTT_USER, MQTT_PASS)
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_HOST, int(MQTT_PORT), 60)

client.loop_forever()