#include "main.h"

// TimerHandle_t tmr1_h = NULL;
// QueueHandle_t xTemp = NULL;

// void createTempQueue()
// {
//   xTemp = xQueueCreate(3, sizeof(temp));
// }

// void get_temp_task(void *arg)
// {
//   while (1)
//   {
//     sensors.requestTemperatures();
//     float temperatureC = sensors.getTempCByIndex(0);
//     xQueueSend(xTemp, (void *)&temperatureC, (TickType_t)0);
//     xQueueSend(xTemp, (void *)&temperatureC, (TickType_t)0);
//     xQueueSend(xTemp, (void *)&temperatureC, (TickType_t)0);
//     Serial.print(temperatureC);
//     Serial.println("ºC");
//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//   }
// }

// void calculate_PWM(void *arg)
// {
//   double Kp = 6, Ki = 5, Kd = 2;
//   PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
//   float temp_rev = 0, last_temp = 0;
//   myPID.SetMode(AUTOMATIC);
//   myPID.SetOutputLimits(100, 1024);
//   while (1)
//   {
//     switch (STATE)
//     {
//     case Running:
//       // Serial.println("Running Lamp");
//       if (xQueueReceive(xTemp, &temp_rev, (TickType_t)0) == pdTRUE)
//       {
//         Input = temp_rev;
//         if (Input == -127)
//           Input = last_temp;
//         // Serial.println(Input);
//         if (Input < Setpoint + 5)
//         {
//           myPID.Compute();
//           // Serial.printf("input: %f, output: %f\n", Input, Output);
//           ledcWrite(lampChannel, (uint32_t)Output);
//         }
//         else if (Input > Setpoint + 5)
//         {
//           ledcWrite(lampChannel, 0);
//         }
//         last_temp = temp_rev;
//       }
//       break;
//     case Stop:
//       ledcWrite(lampChannel, 0);
//       break;
//     }
//     // ledcWrite(lampChannel, 512);
//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//   }
// }

// void fan_PWM(void *arg)
// {
//   double Kp = 6, Ki = 5, Kd = 2;
//   PID fan_PID(&fan_Input, &fan_Output, &Setpoint, Kp, Ki, Kd, DIRECT);
//   float temp_rev = 0;
//   fan_PID.SetMode(AUTOMATIC);
//   fan_PID.SetOutputLimits(50, 255);
//   while (1)
//   {
//     switch (STATE)
//     {
//     case Running:
//       if (xQueueReceive(xTemp, &temp_rev, (TickType_t)0) == pdTRUE)
//       {
//         fan_Input = temp_rev;
//         // Serial.printf("Fan: %f\n", fan_Input);
//         if (fan_Input > (Setpoint + 2) && fan_Input < (Setpoint + 10))
//         {
//           fan_PID.Compute();
//           ledcWrite(fanChannel, fan_Output);
//           Serial.println("Fan is running");
//         }
//         else if (fan_Input > (Setpoint + 10))
//         {
//           ledcWrite(fanChannel, 255);
//         }
//         else if (fan_Input <= Setpoint)
//         {
//           ledcWrite(fanChannel, 0);
//         }
//       }
//       break;

//     case Stop:
//       if (xQueueReceive(xTemp, &temp_rev, (TickType_t)0) == pdTRUE)
//       {
//         if (temp_rev > 35 && temp_rev != -127)
//           ledcWrite(fanChannel, 255);
//         else
//           ledcWrite(fanChannel, 0);
//       }
//       break;
//     }
//     // ledcWrite(fanChannel, 120);
//     vTaskDelay(1010 / portTICK_PERIOD_MS);
//   }
// }