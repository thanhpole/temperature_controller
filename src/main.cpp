#include "main.h"

const char *ssid = "PoPo 1";
const char *password = "supim0404";
float temp = 0, last_temp = 0;
double Setpoint = 0, Input, Output, fan_Output, fan_Input;
state STATE;
double Kp = 6, Ki = 5, Kd = 2;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
PID fan_PID(&fan_Input, &fan_Output, &Setpoint, Kp, Ki, Kd, DIRECT);
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup_wifi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200);
  STATE = Stop;
  WifiManager();
  // setup_wifi();
  sensors.begin();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  connect_to_broker();
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(100, 1024);
  fan_PID.SetMode(AUTOMATIC);
  fan_PID.SetOutputLimits(50, 255);
  analogWriteFreq(freq);
  analogWriteResolution(10);
  temp = sensors.getTempCByIndex(0);
  // ledcAttachPin(lamp, lampChannel);
  // ledcAttachPin(fan, fanChannel);
  // createTempQueue();
  // pinMode(ledPin, OUTPUT);
  // tmr1_h = xTimerCreate(
  //     "TMR1",
  //     1000 / portTICK_PERIOD_MS,
  //     pdTRUE,
  //     NULL,
  //     get_temp_task);
  // if (tmr1_h == NULL)
  // {
  //   Serial.println("Could not create timer");
  // }
  // else
  // xTimerStart(tmr1_h, portMAX_DELAY);
  // xTaskCreate(&calculate_PWM, "PWM", 4096, NULL, 10, NULL);
  // xTaskCreate(&get_temp_task, "temp", 4096, NULL, 11, NULL);
  // xTaskCreate(&fan_PWM, "FAN", 4096, NULL, 9, NULL);
  // xTaskCreate(&mqtt_task, "mqtt", 4096, NULL, 8, NULL);
}

void loop()
{
  client.loop();
  static unsigned long prev_temp_time = 0;
  static unsigned long prev_fan_time = 0;
  static unsigned long prev_mqtt_time = 0;
  static unsigned long prev_lamp_time = 0;

  if ((millis() - prev_temp_time) >= 1000)
  {
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);
    temp = temperatureC;
    Serial.print(temperatureC);
    Serial.println("ºC");
    prev_temp_time = millis();
  }
  switch (STATE)
  {
  case Running:
    // Serial.println("Running Lamp");
    if ((millis() - prev_lamp_time) >= 1000)
    {
      Input = temp;
      if (Input == -127)
        Input = last_temp;
      // Serial.println(Input);
      if (Input < Setpoint + 5)
      {
        myPID.Compute();
        // Serial.printf("input: %f, output: %f\n", Input, Output);
        // ledcWrite(lampChannel, (uint32_t)Output);
        analogWrite(lamp, Output);
      }
      else if (Input > Setpoint + 5)
      {
        analogWrite(lamp, 0);
      }
      prev_lamp_time = millis();
    }
    if ((millis() - prev_fan_time) >= 1000)
    {
      fan_Input = temp;
      analogWriteResolution(8);
      // Serial.printf("Fan: %f\n", fan_Input);
      if (fan_Input > (Setpoint + 2) && fan_Input < (Setpoint + 10))
      {
        fan_PID.Compute();
        analogWrite(fan, fan_Output);
        Serial.println("Fan is running");
      }
      else if (fan_Input > (Setpoint + 10))
      {
        analogWrite(fan, 255);
      }
      else if (fan_Input <= Setpoint)
      {
        analogWrite(fan, 0);
      }
      prev_fan_time = millis();
    }
    last_temp = temp;
    break;
  case Stop:
    analogWrite(lamp, 0);
    if (temp > 35 && temp != -127)
      analogWrite(fan, 255);
    else
      analogWrite(fan, 0);
    break;
  }
  if ((millis() - prev_mqtt_time) >= 1000)
  {
    client.publish(MQTT_CURRENT_TEMP_TOPIC, String(temp).c_str());
    prev_mqtt_time = millis();
  }
}
