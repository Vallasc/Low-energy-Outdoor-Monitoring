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
                device_user = self._mongo_client.get_device_user(payload['id'], payload['token'])
                user = self._mongo_client.get_user(device_user['userId'])
                conf = {}
                for device in user['devices']:
                    if device['id'] == payload['id']:
                        conf['protocol'] = device['protocol']
                        conf['sampleFrequency'] = device['sampleFrequency']
                        conf['configUpdateFrequency'] = device['configUpdateFrequency']
                        conf['minGasValue'] = device['minGasValue']
                        conf['maxGasValue'] = device['maxGasValue']
                        conf['latitude'] = device['latitude']
                        conf['longitude'] = device['longitude']
                        self._mongo_client.set_lastseen_device(payload['id'])
                        break
                self._s.sendto(bytes(json.dumps(conf), 'utf-8'), address)
            except Exception as e:
                logging.error(e)
