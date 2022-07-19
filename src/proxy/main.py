import os
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
import threading
import logging
import time
import coap
import mqtt


INFLUXDB_HOST = os.getenv("INFLUX_HOST", "localhost")
INFLUXDB_PORT = os.getenv("INFLUXDB_PORT", "8086")
INFLUXDB_ORG = os.getenv("INFLUXDB_ORG", "lomo")
INFLUXDB_BUCKET = os.getenv("INFLUXDB_BUCKET", "lomo")
INFLUXDB_TOKEN = os.getenv("INFLUX_TOKEN", "adminadminadmin")

if __name__ == "__main__":

    influx_client_mqtt = InfluxDBClient(
        url= "http://" + INFLUXDB_HOST + ":" + INFLUXDB_PORT,
        token=INFLUXDB_TOKEN,
        org=INFLUXDB_ORG
    )
    write_api_mqtt = influx_client_mqtt.write_api(write_options=SYNCHRONOUS)

    influx_client_coap = InfluxDBClient(
        url= "http://" + INFLUXDB_HOST + ":" + INFLUXDB_PORT,
        token=INFLUXDB_TOKEN,
        org=INFLUXDB_ORG
    )
    write_api_coap = influx_client_coap.write_api(write_options=SYNCHRONOUS)

    logging.basicConfig(level=logging.INFO)

    logging.info("Starting MQTT thread")
    # mqtt_proxy = mqtt.MqttProxy()
    # mqtt_thread = threading.Thread(target=mqtt_proxy.begin, args=(write_api_mqtt, INFLUXDB_BUCKET, INFLUXDB_ORG))
    # mqtt_thread.daemon = True
    # mqtt_thread.start()

    logging.info("Starting CoAP thread")
    coap_proxy = coap.CoapProxy()
    coap_thread = threading.Thread(target=coap_proxy.begin, args=(write_api_coap, INFLUXDB_BUCKET, INFLUXDB_ORG))
    coap_thread.daemon = True
    coap_thread.start()

    while True:
        time.sleep(10)
