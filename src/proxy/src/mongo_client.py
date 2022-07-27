import logging
import os
import pymongo
import calendar
import datetime

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
        
    def get_device_user(self, id, token):
        return self._deviceuser_collection.find_one({'_id': id, 'token': token})

    def get_user(self, user_id):
        return self._user_collection.find_one({'_id': user_id })

    def set_lastseen_device(self, device_id):
        date = datetime.datetime.utcnow()
        utc_time = calendar.timegm(date.utctimetuple())
        self._user_collection.update_one({'devices.id': device_id }, { "$set": { "devices.$.lastSeen": utc_time } })
