import os
from influxdb_client import InfluxDBClient
from influxdb_client import Point as P1
from influxdb_client.client.write_api import SYNCHRONOUS
from datetime import datetime
from meteostat import Daily
from meteostat import Point as P2
import logging
import time



INFLUXDB_HOST = os.getenv("INFLUX_HOST", "localhost")
INFLUXDB_PORT = os.getenv("INFLUXDB_PORT", "8086")
INFLUXDB_ORG = os.getenv("INFLUXDB_ORG", "lomo")
INFLUXDB_BUCKET = os.getenv("INFLUXDB_BUCKET", "lomo")
INFLUXDB_TOKEN = os.getenv("INFLUX_TOKEN", "adminadminadmin")


val = datetime(2022, 7, 18)
print(val)

#ripetere ogni tot tempo

# Create Point for Bologna
location = P2(44.494887, 11.3426163, 54)


data = Daily(location,val,val)
data = data.fetch()

print(data)

outdoor = data.iloc[0]['tavg']

print(outdoor)




def persists(device_id, topic, value):
            print("persist")
            p = P1("devices").tag("id", device_id).field(topic, float(value))
            write_api.write(bucket=INFLUXDB_BUCKET, org=INFLUXDB_ORG, record=p)
            logging.info(p)

            logging.basicConfig(level=logging.INFO)

            def on_connect(client, userdata, flags, rc):
                print("Connected with result code " + str(rc))
                client.subscribe("devices/#")

            def on_message(client, userdata, msg):
                print(msg.topic + " " + str(msg.payload))
                
                print("Msg: "+str(msg))
                print("Payload: "+str(msg.payload))
                print("Topic: "+msg.topic)
                
                msg.payload = outdoor
                print("PayloadOUT: "+str(msg.payload))
                persists(device_id,"outdoor", msg.payload)
                




influx_client = InfluxDBClient(
url= "http://" + INFLUXDB_HOST + ":" + INFLUXDB_PORT,
token=INFLUXDB_TOKEN,
org=INFLUXDB_ORG)
write_api = influx_client.write_api(write_options=SYNCHRONOUS)
    