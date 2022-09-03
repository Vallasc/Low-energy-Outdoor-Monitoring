import logging
import os
import pymongo

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
        self._users_collection = self._db["users"]

    def get_users(self):
        return self._users_collection.find({})