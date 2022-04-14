#if !defined(MAIN_H)
#define MAIN_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "PID_v1.h"
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define oneWireBus (4)
#define lamp (2)
#define fan (5)

#define freq 2000

#define MQTT_SERVER "13.208.186.122"
#define MQTT_PORT 1883
#define MQTT_USER "mqtt"
#define MQTT_PASSWORD "86868686"
#define MQTT_CURRENT_TEMP_TOPIC "CurrentTemp"
#define MQTT_ACTION_TOPIC "action"
#define MQTT_SET_TEMP_TOPIC "SetTemp"

extern WiFiClient espClient;
extern PubSubClient client;
extern OneWire oneWire;
extern DallasTemperature sensors;
// extern TimerHandle_t tmr1_h;
// extern QueueHandle_t xTemp;

enum state
{
  Running,
  Stop
};

extern state STATE;
extern double Setpoint, Input, Output, fan_Output, fan_Input;
extern float temp;

void WifiManager();
void connect_to_broker();
void callback(char *topic, byte *payload, unsigned int length);
// void createTempQueue();
// void calculate_PWM(void *arg);
// void fan_PWM(void *arg);
// void get_temp_task(void *arg);
// void mqtt_task(void *arg);

#endif // MAIN_H
