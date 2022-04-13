#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "PID_v1.h"

const int oneWireBus = 17;
const int lamp = 16;
const int fan = 18;

const int freq = 2000;
const int lampChannel = 0;
const int resolution = 10;
const int fanChannel = 1;
const int fan_resolution = 8;

float temp = 0;
double Setpoint = 60, Input, Output, fan_Output, fan_Input;
QueueHandle_t xTemp = NULL;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
TimerHandle_t tmr1_h = NULL;
void createTempQueue()
{
  xTemp = xQueueCreate(2, sizeof(temp));
}
void get_temp_task(void *arg)
{
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  xQueueSend(xTemp, (void *)&temperatureC, (TickType_t)0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
}

void calculate_PWM(void *arg)
{
  double Kp = 6, Ki = 5, Kd = 1;
  PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
  float temp_rev = 0;
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 1024);
  while (1)
  {
    if (xQueueReceive(xTemp, &temp_rev, (TickType_t)0) == pdTRUE)
    {
      Input = temp_rev;
      myPID.Compute();
      // Serial.printf("input: %f, output: %f\n", Input, Output);
      ledcWrite(lampChannel, (uint32_t)Output);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void fan_PWM(void *arg)
{
  double Kp = 6, Ki = 5, Kd = 1;
  PID fan_PID(&fan_Input, &fan_Output, &Setpoint, Kp, Ki, Kd, DIRECT);
  float temp_rev = 0;
  fan_PID.SetMode(AUTOMATIC);
  fan_PID.SetOutputLimits(0, 255);
  while (1)
  {
    if (xQueueReceive(xTemp, &temp_rev, (TickType_t)0) == pdTRUE)
    {
      fan_Input = temp_rev;
      if (fan_Input > (Setpoint + 3))
      {
        fan_PID.Compute();
        ledcWrite(fanChannel, fan_Output);
        Serial.println("Fan is running");
      }
        }
    vTaskDelay(1050 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);
  sensors.begin();
  ledcSetup(lampChannel, freq, resolution);
  ledcSetup(fanChannel, freq, fan_resolution);
  ledcAttachPin(lamp, lampChannel);
  ledcAttachPin(fan, fanChannel);
  createTempQueue();
  // pinMode(ledPin, OUTPUT);
  tmr1_h = xTimerCreate(
      "TMR1",
      1000 / portTICK_PERIOD_MS,
      pdTRUE,
      NULL,
      get_temp_task);
  if (tmr1_h == NULL)
  {
    Serial.println("Could not create timer");
  }
  else
    xTimerStart(tmr1_h, portMAX_DELAY);
  xTaskCreate(&calculate_PWM, "PWM", 4096, NULL, 10, NULL);
  xTaskCreate(&fan_PWM, "FAN", 4096, NULL, 9, NULL);
}

void loop()
{
}