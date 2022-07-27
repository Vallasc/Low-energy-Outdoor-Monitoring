import os
import paho.mqtt.client as mqtt
import logging
from influx_client import InfluxClient
from mongo_client import MongoClient

MQTT_HOST = os.getenv("MQTT_HOST", "127.0.0.1")
MQTT_PORT = os.getenv("MQTT_PORT", "1883")
MQTT_USER = os.getenv("MQTT_USERNAME", "admin")
MQTT_PASS = os.getenv("MQTT_PASSWORD", "admin")

class MqttProxyWorker:
    def __init__(self):
        self.influx = InfluxClient()
        self._mongo_client = MongoClient()
        logging.basicConfig(level=logging.INFO)

    def on_connect(self, client, userdata, flags, rc):
        logging.info("MQTT connected with result code " + str(rc))
        client.subscribe("devices/#")

    def on_message(self, client, userdata, msg):
        splitted = msg.topic.split("/")
        device_id = splitted[-1]
        # temp/hum/soil/gas/aqi/rssi/lat,lon
        payload = msg.payload.decode("utf-8")
        p = payload.split("/")
        self.influx.save_record(device_id, p[6], p[0], p[1], p[2], p[3], p[4], p[5])
        logging.info("MQTT influx write {device} -> {topic}: {payload}" \
            .format(device=device_id, topic=msg.topic, payload=payload))
        self._mongo_client.set_lastseen_device(device_id)

    def begin(self):
        logging.basicConfig(level=logging.INFO)

        client = mqtt.Client()
        client.username_pw_set(MQTT_USER, MQTT_PASS)
        client.on_connect = self.on_connect
        client.on_message = self.on_message
        client.connect(MQTT_HOST, int(MQTT_PORT), 30)

        client.loop_forever()