#ifndef EVALUATION_H
#define EVALUATION_H

#include <Arduino.h>

RTC_DATA_ATTR unsigned long total_mqtt_packet_count = 0;
RTC_DATA_ATTR unsigned long received_mqtt_packet_count = 0;
RTC_DATA_ATTR unsigned long total_http_packet_count = 0;
RTC_DATA_ATTR unsigned long received_http_packet_count = 0;

#define MISURATION_WINDOW 5

RTC_DATA_ATTR unsigned long mqtt_index = 0;
RTC_DATA_ATTR unsigned long start_ms_mqtt = 0;
RTC_DATA_ATTR unsigned long http_index = 0;
RTC_DATA_ATTR unsigned long mqtt_times[MISURATION_WINDOW];
RTC_DATA_ATTR unsigned long http_times[MISURATION_WINDOW];
RTC_DATA_ATTR unsigned long mqtt_mean_time = 0;
RTC_DATA_ATTR unsigned long http_mean_times = 0;

void init_evaluation_vars()
{
  total_mqtt_packet_count = 0;
  received_mqtt_packet_count = 0;
  total_http_packet_count = 0;
  received_http_packet_count = 0;
  mqtt_mean_time = 0;
  http_mean_times = 0;
  mqtt_index = 0;
  http_index = 0;
  start_ms_mqtt = 0;
  for (int i = 0; i < MISURATION_WINDOW; i++)
  {
    mqtt_times[i] = -1;
    http_times[i] = -1;
  }
}

void calc_mean_mqtt()
{
  int i;
  // mqtt mean
  mqtt_mean_time = 0;
  for (i = 0; i < MISURATION_WINDOW && mqtt_times[i] != -1; i++)
    mqtt_mean_time += mqtt_times[i];
  mqtt_mean_time /= i;
}

void calc_mean_http()
{
  int i;
  // http mean
  http_mean_times = 0;
  for (i = 0; i < MISURATION_WINDOW && http_times[i] != -1; i++){
    http_mean_times += http_times[i];
  }
  http_mean_times /= i;
}

void print_stats()
{
  Serial.println();
  Serial.printf("Received/Total MQTT packets: %lu/%lu\n", received_mqtt_packet_count, total_mqtt_packet_count);
  Serial.printf("MQTT mean travel time: %lu\n", mqtt_mean_time);
  Serial.printf("Received/Total HTTP packets: %lu/%lu\n", received_http_packet_count, total_http_packet_count);
  Serial.printf("HTTP mean travel time: %lu\n", http_mean_times);
  Serial.println();
}

#define START_EVALUATE_MQTT(CODE)                       \
  {                                                     \
    total_mqtt_packet_count++;                          \
    start_ms_mqtt = millis();                           \
    CODE;                                               \
  }

#define STOP_EVALUATE_MQTT                              \
  {                                                     \
    mqtt_times[mqtt_index] = millis() - start_ms_mqtt;  \
    mqtt_index = (mqtt_index + 1) % MISURATION_WINDOW;  \
    calc_mean_mqtt();                                   \
    received_mqtt_packet_count++;                       \
    start_ms_mqtt = 0;                                  \
  }

#define EVALUATE_HTTP(CODE)                             \
  {                                                     \
    total_http_packet_count++;                          \
    unsigned long start_time = millis();                \
    int res = CODE;                                     \
    if(res == 200) {                                    \
      http_times[http_index] = millis() - start_time;   \
      http_index = (http_index + 1) % MISURATION_WINDOW;\
      calc_mean_http();                                 \
      received_http_packet_count++;                     \
    }                                                   \
  }

#endif