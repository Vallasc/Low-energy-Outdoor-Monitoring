import socket
import os
import logging
from mongo_client import MongoClient
from influx_client import InfluxClient
import json

UDP_PORT= os.getenv("UDP_PORT", "9090")

class UDPWorker:

    def __init__(self):
        # Create a UDP socket
        self._s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        # Bind the socket to the port
        server_address = ("0.0.0.0", int(UDP_PORT))
        self._s.bind(server_address)
        self._mongo_client = MongoClient()
        self._influx = InfluxClient()
        logging.basicConfig(level=logging.INFO)

    def begin(self):
        while True:
            try:
                data, address = self._s.recvfrom(4096)
                print(data.decode("utf-8"))
                p = json.loads(data.decode("utf-8"))
                (user, device) = self._mongo_client.get_user_device(p['id'], p['token'])

                if device is None:
                    continue
                
                if p['ping'] is not None:
                    print(p['ping'] )
                    if p['ping'] == True:
                        self._mongo_client.set_device_values(user["_id"], device["id"], p)
                        logging.info("UDP get config {device} -> {p}".format(device=p['id'], p=p))
                        conf = {}
                        conf['protocol'] = device['protocol']
                        conf['sampleFrequency'] = device['sampleFrequency']
                        conf['configUpdateFrequency'] = device['configUpdateFrequency']
                        conf['minGasValue'] = device['minGasValue']
                        conf['maxGasValue'] = device['maxGasValue']
                        conf['latitude'] = device['latitude']
                        conf['longitude'] = device['longitude']
                        conf['enablePerformanceMonitoring'] = device['enablePerformanceMonitoring']
                        self._s.sendto(bytes(json.dumps(conf), 'utf-8'), address)
                    else:
                        self._influx.save_record(p['id'], p['latLong'], p['temp'], p['hum'], p['soil'], p['gas'], p['aqi'], p['rssi'])
                        logging.info("UDP influx write {device} -> {p}".format(device=p['id'], p=p))
                        self._mongo_client.set_last_values(user["_id"], p['id'], p['temp'], p['hum'],
                                                                                p['soil'], p['gas'], p['aqi'], p['rssi'])
                    self._mongo_client.set_lastseen_device(user['_id'], device['id'])
            except Exception as e:
                logging.error(e)
