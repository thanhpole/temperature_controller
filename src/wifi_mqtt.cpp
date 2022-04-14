#include "main.h"

WiFiClient espClient;
PubSubClient client(espClient);

void WifiManager()
{
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  wm.resetSettings();
  // wm.setSTAStaticIPConfig(IPAddress(192, 168, 1, 39), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  bool res;
  res = wm.autoConnect("Demo", "86868686");
  if (!res)
  {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else
  {
    // if you get here you have connected to the WiFi
    Serial.println("Wifi connected :)");
  }
}

void connect_to_broker()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    String clientId = "nodeWiFi32";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD))
    {
      Serial.println("connected");
      client.subscribe(MQTT_ACTION_TOPIC);
      client.subscribe(MQTT_SET_TEMP_TOPIC);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  char chars[length + 1];
  memcpy(chars, payload, length);
  chars[length] = '\0';
  if (strcmp(topic, "action") == 0)
  {
    Serial.println(chars);
    if (*chars == '1')
      STATE = Running;
    if (*chars == '0')
      STATE = Stop;
  }
  if (strcmp(topic, "SetTemp") == 0)
  {
    Setpoint = atof(chars);
    Serial.println(Setpoint);
  }
}

// void mqtt_task(void *arg)
// {

//   float buf, last_buf = 0;
//   while (1)
//   {
//     client.loop();
//     if (xQueueReceive(xTemp, (void *)&buf, (TickType_t)0) == pdTRUE)
//     {
//       if (buf == -127)
//         buf = last_buf;
//       client.publish(MQTT_CURRENT_TEMP_TOPIC, String(buf).c_str());
//       last_buf = buf;
//     }
//     vTaskDelay(1100 / portTICK_PERIOD_MS);
//   }
// }