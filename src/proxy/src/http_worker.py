from flask import Flask, request
import os
import logging
import json
from mongo_client import MongoClient
from influx_client import InfluxClient
from waitress import serve

HTTP_PORT = os.getenv("HTTP_PORT", "8082")

class HttpProxyWorker:

    def __init__(self):
        self._mongo_client = MongoClient()
        self._influx = InfluxClient()
        self._app = Flask("HttpProxyWorker")

    def begin(self):
        logging.basicConfig(level=logging.INFO)

        @self._app.route("/devices/<id>", methods = ['PUT'])
        def put_sensors(id):
            bearer = request.headers.get('Authorization')
            token = bearer.split()[1]

            p = json.loads(request.data.decode("utf-8"))
            device_user = self._mongo_client.get_device_user(id, token)
            if device_user is None:
                return "unauthorized", 401

            try:
                self._influx.save_record(id, p['latLong'], p['temp'], p['hum'], p['soil'], p['gas'], p['aqi'], p['rssi'])
                logging.info("MQTT influx write {device} -> {payload}" \
                        .format(device=id, payload=p))
                self._mongo_client.set_lastseen_device(device_user['userId'], device_user['_id'])
                self._mongo_client.set_last_values(device_user['userId'], device_user['_id'], p['temp'], p['hum'],
                                                                                p['soil'], p['gas'], p['aqi'], p['rssi'])
            except Exception as e:
                logging.error(e)
            return "", 200

        @self._app.route("/devices/<id>/config", methods = ['GET'])
        def put_sensors(id):
            bearer = request.headers.get('Authorization')
            token = bearer.split()[1]

            p = json.loads(request.data.decode("utf-8"))
            device_user = self._mongo_client.get_device_user(id, token)
            if device_user is None:
                return "unauthorized", 401

            try:
                device = self._mongo_client.get_device(device_user['userId'], id)
                conf = {}
                conf['protocol'] = device['protocol']
                conf['sampleFrequency'] = device['sampleFrequency']
                conf['configUpdateFrequency'] = device['configUpdateFrequency']
                conf['minGasValue'] = device['minGasValue']
                conf['maxGasValue'] = device['maxGasValue']
                conf['latitude'] = device['latitude']
                conf['longitude'] = device['longitude']
                conf['enablePerformanceMonitoring'] = device['enablePerformanceMonitoring']
            except Exception as e:
                logging.error(e)
            return json.dumps(conf), 200

        serve(self._app, host="0.0.0.0", port=int(HTTP_PORT))
