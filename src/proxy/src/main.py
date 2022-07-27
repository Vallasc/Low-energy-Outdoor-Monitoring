import threading
import logging
import time

from http_worker import HttpProxyWorker
from mqtt_worker import MqttProxyWorker
from device_config_worker import DeviceConfigProxyWorker

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)

    logging.info("Starting MQTT thread")
    mqtt_proxy = MqttProxyWorker()
    mqtt_thread = threading.Thread(target=mqtt_proxy.begin)
    mqtt_thread.daemon = True
    mqtt_thread.start()

    logging.info("Starting http thread")
    http_proxy = HttpProxyWorker()
    # http_proxy.begin()
    http_thread = threading.Thread(target=http_proxy.begin)
    http_thread.daemon = True
    http_thread.start()

    logging.info("Starting config thread")
    device_config = DeviceConfigProxyWorker()
    device_config_thread = threading.Thread(target=device_config.begin)
    device_config_thread.daemon = True
    device_config_thread.start()

    while True:
        time.sleep(10)
