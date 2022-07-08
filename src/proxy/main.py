import os
import paho.mqtt.client as mqtt
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
import aiocoap.resource as resource
import aiocoap
import asyncio
import logging

INFLUXDB_HOST = os.getenv("INFLUX_HOST", "localhost")
INFLUXDB_PORT = os.getenv("INFLUXDB_PORT", "8086")
INFLUXDB_ORG = os.getenv("INFLUXDB_ORG", "lomo")
INFLUXDB_BUCKET = os.getenv("INFLUXDB_BUCKET", "lomo")
INFLUXDB_TOKEN = os.getenv("INFLUX_TOKEN", "adminadminadmin")

#protocol = "CoAP"

MQTT_HOST = os.getenv("MQTT_HOST", "localhost")
MQTT_PORT = os.getenv("MQTT_PORT", "1883")
MQTT_USER = os.getenv("MQTT_USER", "admin")
MQTT_PASS = os.getenv("MQTT_PASS", "admin")

COAP_PORT = os.getenv("MQTT_PORT", "5683")

max_gas_value = "/72.00"
min_gas_value = "/40.00"
sample_frequency = "/1"
token = "pippo"


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


class DeviceResource(resource.Resource):
    



    def __init__(self):
        super().__init__()

    async def render_put(self, request):
        payload = request.payload.split("/") # token/temp/hum/soil/aqi
        token = payload[0]
        temp = payload[1]
        hum = payload[2]
        soil = payload[3]
        aqi = payload[4]
        #temp = payload[1]
        protocol = "COAP"
        #protocol = "DE"
        # accesso database prelevo il protocollo


    async def render_get(self, request):
        payload = request.payload.split("/") # token/
        token = payload[0]
        protocol = "COAP"
        protocol = "DE"
        payload= token+max_gas_value+min_gas_value+sample_frequency
        # accesso database prelevo il protocollo
        if protocol == "COAP":
            return aiocoap.Message(payload=payload)
        # else
        #     return aiocoap.Message(code=aiocoap., payload=self.state)





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