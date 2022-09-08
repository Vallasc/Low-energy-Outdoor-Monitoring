import threading
import logging
import time

from http_worker import HttpProxyWorker
from mqtt_worker import MqttProxyWorker
from udp_worker import UDPWorker

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)

    logging.info("Starting MQTT thread")
    mqtt_proxy = MqttProxyWorker()
    mqtt_thread = threading.Thread(target=mqtt_proxy.begin)
    mqtt_thread.daemon = True
    mqtt_thread.start()

    logging.info("Starting http thread")
    http_proxy = HttpProxyWorker()
    http_thread = threading.Thread(target=http_proxy.begin)
    http_thread.daemon = True
    http_thread.start()

    logging.info("Starting udp thread")
    udp_worker = UDPWorker()
    udp_worker_thread = threading.Thread(target=udp_worker.begin)
    udp_worker_thread.daemon = True
    udp_worker_thread.start()

    while True:
        time.sleep(10)
