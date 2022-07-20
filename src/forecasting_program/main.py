import os
import pandas as pd
import time
from datetime import datetime
from prophet import Prophet
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS

#client = InfluxDBClient(url="http://localhost:9999", token=token, org=org)
#query_api = client.query_api()
#write_api = client.write_api(write_options=SYNCHRONOUS)


INFLUXDB_HOST = os.getenv("INFLUX_HOST", "localhost")
INFLUXDB_PORT = os.getenv("INFLUXDB_PORT", "8086")
INFLUXDB_ORG = os.getenv("INFLUXDB_ORG", "lomo")
INFLUXDB_BUCKET = os.getenv("INFLUXDB_BUCKET", "lomo")
INFLUXDB_TOKEN = os.getenv("INFLUX_TOKEN", "adminadminadmin")

token = INFLUXDB_TOKEN
bucket = INFLUXDB_BUCKET
org = INFLUXDB_ORG
client = InfluxDBClient(url="http://localhost:" + INFLUXDB_PORT, token=token, org=org)
query_api = client.query_api()
write_api = client.write_api(write_options=SYNCHRONOUS)


query = 'from(bucket: "lomo")'\
        '|> range(start: v.timeRangeStart, stop: v.timeRangeStop)'\
        '|> filter(fn: (r) => r["id"] == "admin")'\
        '|> filter(fn: (r) => r["_field"] == "gas")'\
        '|> filter(fn: (r) => r["_measurement"] == "devices")'\
        '|> aggregateWindow(every: v.windowPeriod, fn: mean, createEmpty: false)'\
        '|> yield(name: "mean")'


result = client.query_api().query(org=INFLUXDB_ORG, query=query)


#Convert the results into a list:
# raw = []
# for table in result:
#     for record in table.records:
#         raw.append((record.get_value(), record.get_time()))
# print(raw[0:5])


#Convert raw data to DataFrame
# print()
# print("=== influxdb query into dataframe ===")
# print()
# df=pd.DataFrame(raw, columns=['y','ds'], index=None)
# df['ds'] = df['ds'].values.astype('<M8[D]')
# df.head()

#Fit the model by instantiating a new Prophet object and passing in the historical DataFrame

# m = Prophet()
# m.fit(df)
#Use the helper method Prophet.make_future_dataframe to prepare your dataframe for forecasting
#365 specifies the number of time series points you'd like to forecast onto 
# future = m.make_future_dataframe(periods=365)

#Make your prediction
#The predict method will assign each row in future a predicted value (yhat). The upper (yhat_upper) and lower (yhat_lower) confidence intervals are also included as a part of the forecast. Columns for components and uncertainty intervals are also included in the forecast, although they aren't displayed here. 

# forecast = m.predict(future)
# forecast[['ds', 'yhat', 'yhat_lower', 'yhat_upper']].tail()