from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
import logging
import os

INFLUXDB_HOST = os.getenv("INFLUX_HOST", "localhost")
INFLUXDB_PORT = os.getenv("INFLUXDB_PORT", "8086")
INFLUXDB_ORG = os.getenv("INFLUXDB_ORG", "lomo")
INFLUXDB_BUCKET = os.getenv("INFLUXDB_BUCKET", "lomo")
INFLUXDB_TOKEN = os.getenv("INFLUX_TOKEN", "adminadminadmin")

OUT_TEMP_FIELD = "out_temp"
ID_TAG= "id"
LAT_LON_TAG= "lat_lon"

class InfluxClient:
    def __init__(self):
        self._client = InfluxDBClient(
            url= "http://" + INFLUXDB_HOST + ":" + INFLUXDB_PORT,
            token=INFLUXDB_TOKEN,
            org=INFLUXDB_ORG
        )
        self._write_api = self._client.write_api(write_options=SYNCHRONOUS)
        logging.basicConfig(level=logging.INFO)
        
    def save_record(self, device_id, temp):
        p = Point("devices") \
            .tag(ID_TAG, device_id)             \
            .field(OUT_TEMP_FIELD, float(temp))
        self._write_api.write(bucket=INFLUXDB_BUCKET, org=INFLUXDB_ORG, record=p)