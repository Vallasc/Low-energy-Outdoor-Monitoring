import os
import pandas as pd
from pandas.tseries.offsets import DateOffset
import time
# import matplotlib.pyplot as plt
import logging
import datetime 
from prophet import Prophet
from influxdb_client import InfluxDBClient, Point, WriteOptions
from influxdb_client.client.write_api import SYNCHRONOUS
# from sklearn.metrics import mean_squared_error

logging.basicConfig(
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level=logging.INFO
)

INFLUXDB_HOST = os.getenv("INFLUX_HOST", "localhost")
INFLUXDB_PORT = os.getenv("INFLUXDB_PORT", "8086")
INFLUXDB_ORG = os.getenv("INFLUXDB_ORG", "lomo")
INFLUXDB_BUCKET = os.getenv("INFLUXDB_BUCKET", "lomo")
INFLUXDB_TOKEN = os.getenv("INFLUX_TOKEN", "adminadminadmin")

TIME_DATASET = 60 * 15
TIME_FORECASTING = 60 * 5
DEVICE_ID = '6313235cdbeac3dbeabbf087'

now = datetime.datetime.now()
START_TIME = (now - datetime.timedelta(seconds=TIME_DATASET)).strftime("%Y-%m-%dT%H:%M:%S.%fZ")
STOP_TIME = now.strftime("%Y-%m-%dT%H:%M:%S.%fZ")


def success_cb(details, data):
    url, token, org = details
    print(url, token, org)
    data = data.decode('utf-8').split('\n')
    print('Total Rows Inserted:', len(data))  

def error_cb(details, data, exception):
    print(exc)

def retry_cb(details, data, exception):
    print('Retrying because of an exception:', exc)    

client = InfluxDBClient(url="http://"+INFLUXDB_HOST+":"+INFLUXDB_PORT, token=INFLUXDB_TOKEN, org=INFLUXDB_ORG)
query_api = client.query_api()
write_api = client.write_api(write_options=SYNCHRONOUS, success_callback=success_cb, error_callback=error_cb)


query = 'from(bucket: "lomo")' \
  ' |> range(start:' +  START_TIME + ', stop:' + STOP_TIME + ')'\
  ' |> filter(fn: (r) => r["_measurement"] == "devices")' \
  ' |> filter(fn: (r) => r["_field"] == "temp")' \
  ' |> filter(fn: (r) => r["id"] == "' + DEVICE_ID +'")'
        
result = client.query_api().query(org=INFLUXDB_ORG, query=query)

# #Convert the results into a list:
raw = []
for table in result:
    for record in table.records:
        raw.append((record.get_value(), record.get_time()))
# logging.info(raw[0:5])


#Convert raw data to DataFrame
logging.info("influxdb query into dataframe")
df = pd.DataFrame(raw, columns=['y','ds'], index=None)
df['ds'] = df['ds'].dt.tz_localize(None)
logging.info(df.head())

# #Fit the model by instantiating a new Prophet object and passing in the historical DataFrame
m = Prophet(changepoint_prior_scale=0.01).fit(df)

# Use the helper method Prophet.make_future_dataframe to prepare your dataframe for forecasting
# periods specifies the number of time series points you'd like to forecast onto 
# freq time between points 

per = int(TIME_FORECASTING / 60)
future = m.make_future_dataframe(periods=per, freq= DateOffset(minutes=1))
forecast = m.predict(future)
forecast['ds'] = forecast.ds.dt.floor('min')
logging.info(forecast.head())

# #Convert the DataFrame to Line Protocol:

cp = forecast[['ds', 'yhat']].copy()

# weather,location=us-midwest temperature=82 1465839830100400200
lines = ['devices,id=' + DEVICE_ID + ' forcasted_temp=' + str(cp["yhat"][d])
         + ' ' + str(int(time.mktime(cp['ds'][d].timetuple()))) + "000000000" for d in range(len(cp))]

print(lines)
_write_client = client.write_api(
                  write_options=WriteOptions(batch_size=1000, flush_interval=10_000,
                                              jitter_interval=2_000, retry_interval=5_000))
_write_client.write(INFLUXDB_BUCKET, INFLUXDB_ORG, lines)
_write_client.__del__()

# # mse = mean_squared_error(df['y'], forecast['yhat'])
# # print('MSE: %f' % mse)