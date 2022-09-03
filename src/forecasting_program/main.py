import os
import pandas as pd
import time
import matplotlib.pyplot as plt
from datetime import datetime
from prophet import Prophet
from influxdb_client import InfluxDBClient, Point,WriteOptions
from influxdb_client.client.write_api import SYNCHRONOUS
from sklearn.metrics import mean_squared_error

INFLUXDB_HOST = os.getenv("INFLUX_HOST", "localhost")
INFLUXDB_PORT = os.getenv("INFLUXDB_PORT", "8086")
INFLUXDB_ORG = os.getenv("INFLUXDB_ORG", "lomo")
INFLUXDB_BUCKET = os.getenv("INFLUXDB_BUCKET", "lomo")
INFLUXDB_TOKEN = os.getenv("INFLUX_TOKEN", "adminadminadmin")


client = InfluxDBClient(url="http://"+INFLUXDB_HOST+":"+INFLUXDB_PORT, token=INFLUXDB_TOKEN, org=INFLUXDB_ORG)
query_api = client.query_api()
write_api = client.write_api(write_options=SYNCHRONOUS)





query = 'from(bucket: "lomo")'\
  ' |> range(start:2022-07-10T15:00:00Z, stop:2022-09-02T15:00:00Z)'\
  ' |> filter(fn: (r) => r["_measurement"] == "devices")' \
  ' |> filter(fn: (r) => r["_field"] == "temp")' \
  ' |> filter(fn: (r) => r["id"] == "admin")'
        


result = client.query_api().query(org=INFLUXDB_ORG, query=query)



#Convert the results into a list:
raw = []
for table in result:
    for record in table.records:
        raw.append((record.get_value(), record.get_time()))
print(raw[0:5])


#Convert raw data to DataFrame
print()
print("=== influxdb query into dataframe ===")
print()
df=pd.DataFrame(raw, columns=['y','ds'], index=None)
print("Prima ds: ")
print(df['ds'])
df['ds'] = df['ds'].dt.tz_localize(None)
print("y: ")
print(df['y'])
print("Dopo ds: ")
print(df['ds'])
df['ds'] = df['ds'].values.astype('<M8[D]')
#df.groupby('ds').agg()
df.head()
print(df)

#Fit the model by instantiating a new Prophet object and passing in the historical DataFrame

m = Prophet(changepoint_prior_scale=0.01).fit(df)




#Use the helper method Prophet.make_future_dataframe to prepare your dataframe for forecasting
#365 specifies the number of time series points you'd like to forecast onto 



future = m.make_future_dataframe(periods=30,freq='H')

#Make your prediction
#The predict method will assign each row in future a predicted value (yhat). The upper (yhat_upper) and lower (yhat_lower) confidence intervals are also included as a part of the forecast. Columns for components and uncertainty intervals are also included in the forecast, although they aren't displayed here. 


forecast = m.predict(future)

fig = m.plot(forecast)

#fig = m.plot_components(forecast)



#fig.show()

forecast[['ds', 'yhat', 'yhat_lower', 'yhat_upper']].tail()


#Add a measurement column to our DataFrame:

forecast['measurement'] = "devices"

#Convert the DataFrame to Line Protocol:


cp = forecast[['ds', 'yhat', 'yhat_lower', 'yhat_upper','measurement']].copy()
lines = [str(cp["measurement"][d]) 

         + ",id=admin" 
         + " " 
         + "yhat=" + str(cp["yhat"][d]) + ","
         + "yhat_lower=" + str(cp["yhat_lower"][d]) + ","
         + "yhat_upper=" + str(cp["yhat_upper"][d])
         + " " + str(int(time.mktime(cp['ds'][d].timetuple()))) + "000000000" for d in range(len(cp))]

#Write the lines to your instance:



_write_client = client.write_api(write_options=WriteOptions(batch_size=1000, 
                                                            flush_interval=10_000,
                                                            jitter_interval=2_000,
                                                            retry_interval=5_000))


_write_client.write(INFLUXDB_BUCKET, INFLUXDB_ORG, lines)

print("yhat:")
print(forecast['yhat'])


# mse = mean_squared_error(df['y'], forecast['yhat'])
# print('MSE: %f' % mse)



fig.savefig('books_read.png')

#Close the client:

_write_client.__del__()
client.__del__()

