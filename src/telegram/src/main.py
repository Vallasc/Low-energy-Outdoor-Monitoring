import logging
import os
import json
import telebot
import requests
import time
from threading import Thread
from mongo_client import MongoClient
from flask import Flask, request
from waitress import serve

LOMO_API_HOST = os.getenv('LOMO_API_HOST', '127.0.0.1:8888')
TELEGRAM_TOKEN = os.getenv('TELEGRAM_TOKEN', '')
HTTP_PORT = os.getenv("HTTP_PORT", "8087")
TIME_BETWEEN_REPORTS  = int(os.getenv("TIME_BETWEEN_REPORTS", 10)) * 60
logging.basicConfig(
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level=logging.INFO
)

bot = telebot.TeleBot(TELEGRAM_TOKEN, parse_mode=None)

mongo_client = MongoClient()
app = Flask("TelegramManager")

def start_webserver():
    serve(app, host="0.0.0.0", port=int(HTTP_PORT))

@app.route("/users", methods = ['POST'])
def create_user():
    payload = json.loads(request.data.decode("utf-8"))
    user = mongo_client.create_telegram_user(payload["userId"], payload["token"])
    return "", 200

@app.route("/users/<user_id>/messages", methods = ['POST'])
def send_message(user_id):
    payload = json.loads(request.data.decode("utf-8"))
    if payload['telegramToken'] == None: return "", 401
    user = mongo_client.get_telegram_user(user_id, payload['telegramToken'])
    print("Send alert message to user: " + user_id)
    logging.info("Send alert message to user: " + user_id)
    bot.send_message(user['chatId'], payload['message'])
    return "", 200

@bot.message_handler(commands=['start'])
def handle_start(message):
    if message.content_type == 'text':
        logging.info("New user started: " + message.from_user.username)
        args = message.text.split()
        if len(args) == 2:
            user = mongo_client.get_telegram_user_by_token(args[1])
            mongo_client.update_telegram_chatid(args[1], message.chat.id)
            bot.send_message(message.chat.id, "I'm a the LOMO bot, I can hep you to mange your devices!")
            bot.send_message(message.chat.id, "Hello " + user['_id'])
            bot.send_message(message.chat.id, "You can list your devices using /list command")
        else:
            bot.send_message(message.chat.id, "I'm a the LOMO bot, I can help you to manage your devices!\n Go to the telegram section of LOMO webapp")

@bot.message_handler(commands=['stop'])
def handle_stop(message):
    if message.content_type == 'text':
        args = message.text.split()
        mongo_client.delete_telegram_user(message.chat.id)
        bot.send_message(message.chat.id, "User disconnected")

@bot.message_handler(commands=['list'])
def handle_list(message):
    if message.content_type == 'text':
        user = mongo_client.get_telegram_user_by_chat(message.chat.id)
        payload = {'telegramToken': user['telegramToken'], 'userId': user['_id']}
        r = requests.get('http://' + LOMO_API_HOST + '/users/api/telegram', params=payload)
        devices = r.json()['devices']
        msg = "Your devices:\n"
        index = 0
        for device in devices:
            msg += str(index) + ' - ' + device['name'] + '\n'
            index += 1
        bot.send_message(message.chat.id, msg)
        bot.send_message(message.chat.id, "You can show a specific device using /show <DEVICE_NUMBER>")

@bot.message_handler(commands=['show'])
def handle_show(message):
    if message.content_type == 'text':
        logging.info("New user started: " + message.from_user.username)
        args = message.text.split()
        if len(args) == 2:
            i = int(args[1]) if args[1].isdigit() else None
            if i != None:
                user = mongo_client.get_telegram_user_by_chat(message.chat.id)
                payload = {'telegramToken': user['telegramToken'], 'userId': user['_id']}
                r = requests.get('http://' + LOMO_API_HOST + '/users/api/telegram', params=payload)
                devices = r.json()['devices']
                msg = "Device: " + devices[i]['name'] + '\n'
                msg += "- id: " + devices[i]['id'] + '\n'
                msg += "- protocol: " + devices[i]['protocol'] + '\n'
                msg += "- sample frequency: " + str(devices[i]['sampleFrequency']) + '\n'
                msg += "- config update frequency: " + str(devices[i]['configUpdateFrequency']) + '\n'
                msg += "- minGasValue: " + str(devices[i]['configUpdateFrequency']) + '\n'
                msg += "- maxGasValue: " + str(devices[i]['configUpdateFrequency']) + '\n'
                msg += "- latitude: " + str(devices[i]['latitude']) + '\n'
                msg += "- longitude: " + str(devices[i]['longitude']) + '\n'
                msg += "- wifi ssid: " + devices[i]['wifiSsid'] + '\n\n'
                msg += "Latest sensors values:\n"
                msg += "- temperature: " + str(devices[i]['lastTempValue']) + '\n'
                msg += "- humidity: " + str(devices[i]['lastHumidityValue']) + '\n'
                msg += "- soil: " + str(devices[i]['lastSoilValue']) + '\n'
                msg += "- gas: " + str(devices[i]['lastGasValue']) + '\n'
                msg += "- AQI: " + str(devices[i]['lastAqiValue']) + '\n'
                msg += "- RSSI: " + str(devices[i]['lastRssiValue'])
                bot.send_message(message.chat.id, msg)
                return
        bot.send_message(message.chat.id, "Usage /show <DEVICE_NUMBER>")

@bot.message_handler(commands=['enablereport'])
def handle_show(message):
    mongo_client.change_report_flag(message.chat.id, True)
    bot.send_message(message.chat.id, "Reports enbled")

@bot.message_handler(commands=['disablereport'])
def handle_show(message):
    mongo_client.change_report_flag(message.chat.id, False)
    bot.send_message(message.chat.id, "Reports disabled")

def send_reports():
    users = mongo_client.get_all_telegram_users()
    for user in users:
        if user['reportAlert']:
            payload = {'telegramToken': user['telegramToken'], 'userId': user['_id']}
            r = requests.get('http://' + LOMO_API_HOST + '/users/api/telegram', params=payload)
            devices = r.json()['devices']
            msg = "Devices report:\n\n"
            for device in devices:
                msg += "Device: " + device['name'] + '\n'
                msg += "- id: " + device['id'] + '\n'
                msg += "- temperature: " + str(device['lastTempValue']) + '\n'
                msg += "- humidity: " + str(device['lastHumidityValue']) + '\n'
                msg += "- soil: " + str(device['lastSoilValue']) + '\n'
                msg += "- gas: " + str(device['lastGasValue']) + '\n'
                msg += "- AQI: " + str(device['lastAqiValue']) + '\n'
                msg += "- RSSI: " + str(device['lastRssiValue']) + '\n\n'
            bot.send_message(user['chatId'], msg)
    time.sleep(TIME_BETWEEN_REPORTS)

if __name__ == '__main__':
    Thread(target=start_webserver).start()
    Thread(target=send_reports).start()
    bot.infinity_polling()