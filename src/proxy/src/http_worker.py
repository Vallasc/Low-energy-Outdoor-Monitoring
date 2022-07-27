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
            if self._mongo_client.get_device_user(id, token) is None:
                return "unauthorized", 401

            try:
                self._influx.save_record(id, p['latLong'], p['temp'], p['hum'], p['soil'], p['gas'], p['aqi'], p['rssi'])
                logging.info("MQTT influx write {device} -> {payload}" \
                        .format(device=id, payload=p))
                self._mongo_client.set_lastseen_device(id)
            except Exception as e:
                logging.error(e)
            return "", 200

        serve(self._app, host="0.0.0.0", port=int(HTTP_PORT))
