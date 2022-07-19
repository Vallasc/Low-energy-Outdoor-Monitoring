import os
import paho.mqtt.client as mqtt
from influxdb_client import WriteApi, Point
import logging

MQTT_HOST = os.getenv("MQTT_HOST", "localhost")
MQTT_PORT = os.getenv("MQTT_PORT", "1883")
MQTT_USER = os.getenv("MQTT_USER", "admin")
MQTT_PASS = os.getenv("MQTT_PASS", "admin")

class MqttProxy:

    def on_connect(self, client, userdata, flags, rc):
        print("Connected with result code " + str(rc))
        client.subscribe("devices/#")

    def on_message(self, client, userdata, msg):
        splitted = msg.topic.split("/")
        topic = splitted[-1]
        device_id = splitted[-2]

        if topic == "temp" or topic == "hum" or topic == "aqi" or topic == "soil"or topic == "gas":
            p = Point("devices").tag("id", device_id).field(topic, float(msg.payload))
            self.influx_api.write(bucket=self.influx_bucket, org=self.influx_org, record=p)
            logging.info("MQTT influx write {device} -> {topic}: {payload}".format(device=device_id, topic=topic, payload=msg.payload))

    def begin(self, write_api, bucket, org):
        logging.basicConfig(level=logging.INFO)
        self.influx_api = write_api
        self.influx_bucket = bucket
        self.influx_org = org

        client = mqtt.Client()
        client.username_pw_set(MQTT_USER, MQTT_PASS)
        client.on_connect = self.on_connect
        client.on_message = self.on_message
        client.connect(MQTT_HOST, int(MQTT_PORT), 60)

        client.loop_forever()