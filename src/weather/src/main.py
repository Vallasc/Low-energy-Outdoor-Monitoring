import logging
import requests
import os
import time
from mongo_client import MongoClient
from influx_client import InfluxClient

logging.basicConfig(
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level=logging.INFO
)

API_KEY= os.getenv("API_KEY", "")
SLEEP_TIME= int(os.getenv("SLEEP_TIME", "1800")) 

if __name__ == '__main__':
    mongo_client = MongoClient()
    influx = InfluxClient()
    users = mongo_client.get_users()

    while True:
        for user in users:
            for device in user['devices']:
                try:
                    r = requests.get('http://api.openweathermap.org/data/2.5/weather?units=metric'
                        + '&lat=' + str(device['latitude']) + '&lon=' + str(device['longitude']) + '&appid=' + API_KEY)
                    results = r.json()
                    if results['cod'] == 200:
                        influx.save_record(device['id'], str(device['latitude']) + ',' + str(device['longitude']), results['main']['temp'])
                        logging.info('device ' + str(device['id']) + ' -> ' + str(results['main']['temp']))
                except Exception:
                    logging.error("Error getting weather of " + str(device['id']))
        time.sleep(SLEEP_TIME)