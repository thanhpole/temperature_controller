# closed-box-heating

## 1. Technology used

- **Programming Language:** C++
- **Cloud:** AWS EC2 Free tier
- **Tools:** Node-RED, nodejs, MQTT broker
- **Board:** ESP8266
  
## 2. Project description

This is a small box that controls the temperature inside a box of 10cm^2 with an incandescent lamp, temperature sensor, and a fan through the web browser.

## 3. Detail

- Create an MQTT broker on the EC2 machine also install Node-RED
- Create a GUI by using Node-RED which shows the state (ON/OFF), current temperature, and the controller of the box 
- Connect the Node-RED GUI to each controller node (set temperature, get temperature, set status (on/off))
- Apply the PID algorithm to control the temperature of the box. Using PWM to control the lamp voltage and the fan voltage. If the sensor gets the temperate higher than the set temp, then the fan will run, and the lamp bright darker. If the current temperate lower than the set temp then the lamp will be brighter and the fan turn off
- Config the wifi and MQTT broker connection for the ESP8266 board
- Update realtime the current status of the box (On/Off, temperature)
  
## 4. Result

The box works as expected. Error is less than 3 degrees Celcius.
