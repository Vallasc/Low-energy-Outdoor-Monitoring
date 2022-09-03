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
        self._telegramuser_collection = self._db["telegramusers"]
        
    def create_telegram_user(self, userid, token):
        self._telegramuser_collection.insert_one({
            '_id': userid,
            'telegramToken': token,
            'chatId': None,
            'reportAlert': False
        })

    def update_telegram_chatid(self, token, chatid):
        return self._telegramuser_collection.update_one(
            {'telegramToken': token},
            { "$set": { 'chatId': chatid } })

    def delete_telegram_user(self, chat_id):
        self._telegramuser_collection.delete_one({
            'chatId': chat_id
        })

    def get_telegram_user(self, userid, token):
        return self._telegramuser_collection.find_one({'_id': userid, 'telegramToken': token})
    
    def get_telegram_user_by_token(self, token):
        return self._telegramuser_collection.find_one({'telegramToken': token})
    
    def get_telegram_user_by_chat(self, chat_id):
        return self._telegramuser_collection.find_one({'chatId': chat_id})

    def get_all_telegram_users(self):
        return self._telegramuser_collection.find({})

    def change_report_flag(self, chat_id, flag):
        return self._telegramuser_collection.update_one(
            {'chatId': chat_id},
            { "$set": { 'reportAlert': flag } })