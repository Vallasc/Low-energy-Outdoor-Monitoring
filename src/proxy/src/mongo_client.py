import logging
import os
import pymongo
import time

MONGO_HOST = os.getenv("MONGO_HOST", "127.0.0.1")
MONGO_USERNAME= os.getenv("MONGO_USERNAME", "admin")
MONGO_PASSWORD= os.getenv("MONGO_PASSWORD", "admin")
MONGO_DB_NAME= os.getenv("MONGO_DB_NAME", "lomo")
MONGO_PORT= os.getenv("MONGO_PORT", "27017")

class MongoClient:
    def __init__(self):
        self._mongo_client = pymongo.MongoClient('mongodb://%s:%s@%s:%s' % (MONGO_USERNAME, MONGO_PASSWORD, MONGO_HOST, MONGO_PORT))
        self._db = self._mongo_client[MONGO_DB_NAME]
        logging.basicConfig(level=logging.INFO)
        self._deviceuser_collection = self._db["devicesusers"]
        self._user_collection = self._db["users"]
        
    def get_device_user_token(self, id, token):
        return self._deviceuser_collection.find_one({'_id': id, 'token': token})
    
    def get_device_user(self, id):
        return self._deviceuser_collection.find_one({'_id': id})

    def get_user_device(self, id, token):
        device_user = self._deviceuser_collection.find_one({'_id': id, 'token': token})
        if device_user is not None:
            user = self.get_user(device_user["userId"])
            for device in user["devices"]:
                if device['id'] == id:
                    return (user, device)
        return (None, None)

    def get_device(self, userd_id, device_id):
        user = self.get_user(userd_id)
        for device in user["devices"]:
            if device['id'] == device_id:
                return device
        return None

    def get_user(self, user_id):
        return self._user_collection.find_one({'_id': user_id })

    def set_last_values(self, user_id, device_id, temp, hum, soil, gas, aqi, rssi):
        self._user_collection.update_one({'_id': user_id, 'devices.id': device_id }, 
                    { "$set": { 
                        "devices.$.lastTempValue": float(temp),
                        "devices.$.lastHumidityValue": float(hum),
                        "devices.$.lastGasValue": float(gas),
                        "devices.$.lastSoilValue": float(soil),
                        "devices.$.lastAqiValue": float(aqi),
                        "devices.$.lastRssiValue": float(rssi)
                    } })

    def set_lastseen_device(self, user_id, device_id):
        epoch_time = int(time.time())
        self._user_collection.update_one({'_id': user_id, 'devices.id': device_id }, { "$set": { "devices.$.lastSeen": epoch_time } })

    def set_device_values(self, user_id, device_id, json):
        self._user_collection.update_one({
                '_id': user_id, 
                'devices.id': device_id 
            }, { 
                "$set": {
                    "devices.$.host": json.get("host", ""),
                    "devices.$.wifiSsid": json.get("wifiSsid", ""),
                    "devices.$.totalMqttPacketCount": json.get("totalMqttPacketCount", 0),
                    "devices.$.receivedMqttPacketCount": json.get("receivedMqttPacketCount", 0),
                    "devices.$.totalHttpPacketCount": json.get("totalHttpPacketCount", 0),
                    "devices.$.receivedHttpPacketCount": json.get("receivedHttpPacketCount", 0),
                    "devices.$.mqttMeanTime": json.get("mqttMeanTime", 0),
                    "devices.$.httpMeanTime": json.get("httpMeanTime", 0)
                }
            })
