import os
import pandas as pd
from pandas.tseries.offsets import DateOffset
import time
import logging
import datetime 
from prophet import Prophet
from influxdb_client import InfluxDBClient, Point, WriteOptions
from influxdb_client.client.write_api import WriteType
from mongo_client import MongoClient

logging.basicConfig(
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level=logging.INFO
)

INFLUXDB_HOST = os.getenv("INFLUX_HOST", "localhost")
INFLUXDB_PORT = os.getenv("INFLUXDB_PORT", "8086")
INFLUXDB_ORG = os.getenv("INFLUXDB_ORG", "lomo")
INFLUXDB_BUCKET = os.getenv("INFLUXDB_BUCKET", "lomo")
INFLUXDB_TOKEN = os.getenv("INFLUX_TOKEN", "adminadminadmin")

SLEEP_TIME = int(os.getenv("SLEEP_TIME", "10"))
FIELDS = [ 'temp', 'gas', 'hum', 'soil']

def result_to_dataframe(result):
    raw = []
    for table in result:
        for record in table.records:
            raw.append((record.get_value(), record.get_time()))
    return pd.DataFrame(raw, columns=['y','ds'], index=None)

if __name__ == '__main__':
    mongo_client = MongoClient()

    client = InfluxDBClient(url="http://"+INFLUXDB_HOST+":"+INFLUXDB_PORT, token=INFLUXDB_TOKEN, org=INFLUXDB_ORG)

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
                            START_TIME = (now - datetime.timedelta(minutes=device['trainingTime'])).strftime("%Y-%m-%dT%H:%M:%S.%fZ")
                            STOP_TIME = now.strftime("%Y-%m-%dT%H:%M:%S.%fZ") 
                            query = 'from(bucket: "lomo")' \
                                ' |> range(start:' +  START_TIME + ', stop:' + STOP_TIME + ')'\
                                ' |> filter(fn: (r) => r["_measurement"] == "devices")' \
                                ' |> filter(fn: (r) => r["_field"] == "' + field + '")' \
                                ' |> filter(fn: (r) => r["id"] == "' + str(device['id']) +'")'
                                    
                            result = client.query_api().query(org=INFLUXDB_ORG, query=query)

                            # #Convert the results to dataframe
                            df = result_to_dataframe(result)
                            df['ds'] = df['ds'].dt.tz_localize(None)
                            # logging.info(df.head())

                            # Fit the model by instantiating a new Prophet object and passing in the historical DataFrame
                            m = Prophet(changepoint_prior_scale=0.01).fit(df)

                            # Use the helper method Prophet.make_future_dataframe to prepare your dataframe for forecasting
                            # periods specifies the number of time series points you'd like to forecast onto 
                            # freq time between points 
                            per = int(device['predictionTime'])
                            future = m.make_future_dataframe(periods=per, freq= DateOffset(minutes=1))
                            forecast = m.predict(future)
                            forecast['ds'] = forecast.ds.dt.floor('min')
                            # logging.info(forecast.head())
                            # Convert the DataFrame to Line Protocol:
                            cp = forecast[['ds', 'yhat']].copy()
                            lines = ['devices,id=' + str(device['id']) + ' forecasted_' + field + '=' + str(cp["yhat"][d])
                                    + ' ' + str(int(time.mktime(cp['ds'][d].timetuple()))) + "000000000" for d in range(len(cp))]
                            write_client = client.write_api(
                                                write_options=WriteOptions(batch_size=1000, flush_interval=10_000,
                                                            jitter_interval=2_000, retry_interval=5_000, write_type=WriteType.synchronous))
                            write_client.write(INFLUXDB_BUCKET, INFLUXDB_ORG, lines)
                            write_client.__del__()

                            # mse = mean_squared_error(df['y'], forecast['yhat'])
                            # print('MSE: %f' % mse)
                        except Exception:
                            logging.error("Error processing " + str(device['id']))
        time.sleep(SLEEP_TIME)