import os
import pandas as pd
from pandas.tseries.offsets import DateOffset
import time
import logging
import datetime 
from prophet import Prophet
from influxdb_client import InfluxDBClient, Point, WriteOptions
from influxdb_client.client.write_api import WriteType
from sklearn.metrics import mean_squared_error, r2_score
from sklearn.model_selection import train_test_split
from mongo_client import MongoClient

logging.basicConfig(
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level=logging.INFO
)

# Influx credentials
INFLUXDB_HOST = os.getenv("INFLUX_HOST", "localhost")
INFLUXDB_PORT = os.getenv("INFLUXDB_PORT", "8086")
INFLUXDB_ORG = os.getenv("INFLUXDB_ORG", "lomo")
INFLUXDB_BUCKET = os.getenv("INFLUXDB_BUCKET", "lomo")
INFLUXDB_TOKEN = os.getenv("INFLUX_TOKEN", "adminadminadmin")

SLEEP_TIME = int(os.getenv("SLEEP_TIME", "10"))
FIELDS = [ 'temp', 'gas', 'hum', 'soil']

mongo_client = MongoClient()
client = InfluxDBClient(url="http://"+INFLUXDB_HOST+":"+INFLUXDB_PORT, token=INFLUXDB_TOKEN, org=INFLUXDB_ORG)

# Convert query result into a dataframe
def result_to_dataframe(result):
    raw = []
    for table in result:
        for record in table.records:
            raw.append((record.get_value(), record.get_time()))
    return pd.DataFrame(raw, columns=['y','ds'], index=None)

# Calculate forecasting of a specific device and field
def calc_forecasting(device, field, start_time, stop_time):
    query = 'from(bucket: "' + INFLUXDB_BUCKET + '")' \
        ' |> range(start:' +  start_time + ', stop:' + stop_time + ')'\
        ' |> filter(fn: (r) => r["_measurement"] == "devices")' \
        ' |> filter(fn: (r) => r["_field"] == "' + field + '")' \
        ' |> filter(fn: (r) => r["id"] == "' + str(device['id']) +'")'
            
    result = client.query_api().query(org=INFLUXDB_ORG, query=query)
    # Convert the results to dataframe
    df = result_to_dataframe(result)
    df['ds'] = df['ds'].dt.tz_localize(None)
    # Fit the model by instantiating a new Prophet object and passing in the historical DataFrame
    m = Prophet(
        yearly_seasonality=False,
        weekly_seasonality=False,
        daily_seasonality=30,
        n_changepoints=35,
        changepoint_range=1,
        changepoint_prior_scale=0.01
        # interval_width=1.0
    )
    m.fit(df)
    # periods specifies the number of time series points you'd like to forecast onto 
    # freq time between points 
    per = int(device['predictionTime'])
    future = m.make_future_dataframe(periods=per, freq= DateOffset(minutes=1))
    forecast = m.predict(future)
    # truncate ds to minutes
    forecast['ds'] = forecast.ds.dt.floor('min')
    lines = ['devices,id=' + str(device['id']) + ' forecasted_' + field + '=' + str(forecast["yhat"][d])
                                    + ' ' + str(int(time.mktime(forecast['ds'][d].timetuple()))) + "000000000" for d in range(len(forecast))]
    write_client = client.write_api(write_options=WriteOptions(batch_size=1000, flush_interval=10_000,
                                                            jitter_interval=2_000, retry_interval=5_000, write_type=WriteType.synchronous))
    # Write data on influx
    write_client.write(INFLUXDB_BUCKET, INFLUXDB_ORG, lines)
    write_client.__del__()

if __name__ == '__main__':
    while True:
        users = mongo_client.get_users()
        for user in users:
            for device in user['devices']:
                if device['enablePrediction']:
                    logging.info('Processing device ' + str(device['id']))
                    for field in FIELDS:
                        logging.info('Field ' + field)
                        try:
                            now = datetime.datetime.now()
                            START_TIME = '-5y'
                            if device['trainingTime'] > 0:
                                START_TIME = (now - datetime.timedelta(minutes=device['trainingTime'])).strftime("%Y-%m-%dT%H:%M:%S.%fZ")
                            STOP_TIME = now.strftime("%Y-%m-%dT%H:%M:%S.%fZ") 
                            
                            calc_forecasting(device, field, START_TIME, STOP_TIME)
                        except Exception:
                            logging.error("Error processing " + str(device['id']))
        time.sleep(SLEEP_TIME)