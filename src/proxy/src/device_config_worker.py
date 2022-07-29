import socket
import os
import logging
from mongo_client import MongoClient
import json

UDP_PORT= os.getenv("UDP_PORT", "9090")

class DeviceConfigProxyWorker:

    def __init__(self):
        # Create a UDP socket
        self._s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        # Bind the socket to the port
        server_address = ("0.0.0.0", int(UDP_PORT))
        self._s.bind(server_address)
        self._mongo_client = MongoClient()
        logging.basicConfig(level=logging.INFO)

    def begin(self):
        while True:
            try:
                data, address = self._s.recvfrom(4096)
                payload = json.loads(data.decode("utf-8"))
                (user, device) = self._mongo_client.get_user_device(payload['id'], payload['token'])
                self._mongo_client.set_device_values(user["_id"], device["id"], payload)

                conf = {}
                conf['protocol'] = device['protocol']
                conf['sampleFrequency'] = device['sampleFrequency']
                conf['configUpdateFrequency'] = device['configUpdateFrequency']
                conf['minGasValue'] = device['minGasValue']
                conf['maxGasValue'] = device['maxGasValue']
                conf['latitude'] = device['latitude']
                conf['longitude'] = device['longitude']
                conf['enablePerformanceMonitoring'] = device['enablePerformanceMonitoring']
                
                self._mongo_client.set_lastseen_device(user['_id'], device['id'])
                self._s.sendto(bytes(json.dumps(conf), 'utf-8'), address)
            except Exception as e:
                logging.error(e)
