import aiocoap.resource as resource
import aiocoap
import asyncio
from influxdb_client import WriteApi, Point
import os
import logging

COAP_PORT = os.getenv("COAP_PORT", "5683")

class DeviceResource(resource.Resource):

    def __init__(self, influx_api, influx_bucket, influx_org):
        self.influx_api = influx_api
        self.influx_bucket = influx_bucket
        self.influx_org = influx_org
        super().__init__()

    async def render_put(self, request):
        payload = request.payload.decode("utf-8").split("/") # token/temp/hum/soil/gas/aqi
        token = payload[0]
        temp = payload[1]
        hum = payload[2]
        soil = payload[3]
        gas = payload[4]
        aqi = payload[5]

        # Controllare che utente sia loggato e prendewre device id
        device_id = "admin"
        p = Point("devices").tag("id", device_id)   \
            .field("temp", float(temp)) \
            .field("hum", float(hum)) \
            .field("soil", float(soil)) \
            .field("gas", float(gas)) \
            .field("aqi", float(aqi)) \

        self.influx_api.write(bucket=self.influx_bucket, org=self.influx_org, record=p)
        logging.info("COAP influx write {device} -> {t} / {h} / {s} / {g} / {a}" \
            .format(device=device_id, t=temp, h=hum, s=soil, g=gas, a=aqi))
        return aiocoap.Message(code=aiocoap.CHANGED)

    # async def render_get(self, request):
    #     payload = request.payload.split("/") # token/
    #     token = payload[0]
    #     protocol = "COAP"
    #     protocol = "DE"
    #     payload= token+max_gas_value+min_gas_value+sample_frequency
    #     # accesso database prelevo il protocollo
    #     if protocol == "COAP":
    #         return aiocoap.Message(payload=payload)
    #     # else
    #     #     return aiocoap.Message(code=aiocoap., payload=self.state)

class CoapProxy:

    def begin(self, write_api, bucket, org):
        logging.basicConfig(level=logging.INFO)
        self.influx_api = write_api
        self.influx_bucket = bucket
        self.influx_org = org

        root = resource.Site()
        root.add_resource(['devices'], DeviceResource(self.influx_api, self.influx_bucket, self.influx_org))

        try:
            loop = asyncio.get_event_loop()
        except RuntimeError as e:
            if str(e).startswith('There is no current event loop in thread'):
                loop = asyncio.new_event_loop()
                asyncio.set_event_loop(loop)
            else:
                raise
        #asyncio.Task(aiocoap.Context.create_server_context(root, bind=('0.0.0.0', 5683))) # not working on Windows
        asyncio.Task(aiocoap.Context.create_server_context(root, bind=('192.168.1.229', 5683)))

        loop.run_forever()