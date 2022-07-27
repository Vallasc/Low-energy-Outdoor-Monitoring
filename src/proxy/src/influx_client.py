from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
import logging
import os

INFLUXDB_HOST = os.getenv("INFLUX_HOST", "localhost")
INFLUXDB_PORT = os.getenv("INFLUXDB_PORT", "8086")
INFLUXDB_ORG = os.getenv("INFLUXDB_ORG", "lomo")
INFLUXDB_BUCKET = os.getenv("INFLUXDB_BUCKET", "lomo")
INFLUXDB_TOKEN = os.getenv("INFLUX_TOKEN", "adminadminadmin")

TEMP_FIELD = "temp"
HUM_FIELD = "hum"
GAS_FIELD = "gas"
AQI_FIELD = "aqi"
SOIL_FIELD = "soil"
RSSI_FIELD = "rssi"
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
        
    def save_record(self, device_id, lat_lon, temp, hum, soil, gas, aqi, rssi):
        p = Point("devices") \
            .tag(ID_TAG, device_id)             \
            .tag(LAT_LON_TAG, lat_lon)          \
            .field(TEMP_FIELD, float(temp))     \
            .field(HUM_FIELD, float(hum))       \
            .field(GAS_FIELD, float(gas))       \
            .field(AQI_FIELD, float(aqi))       \
            .field(SOIL_FIELD, float(soil))     \
            .field(RSSI_FIELD, float(rssi))     
        self._write_api.write(bucket=INFLUXDB_BUCKET, org=INFLUXDB_ORG, record=p)