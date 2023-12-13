sensor: An implementation of an automated plant watering system
========================================================================
**sensor** is an automated plant system with an ESP32 with Wi-Fi, 
soil moisture sensors, and an LED strip.

Requirements
------------
In order to build **sensor** you need all the dependencies for the ESP32 by following [this guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/).

**Clone this repository to esp:**\
```cd /Users/<user>/esp```\
```git clone https://github.com/yunu121/sensor.git```

Flashing to ESP32
-----------------
Open sensor.c and adjust values from lines 22-29 depending on your setup.\
Make sure to calibrate the values by testing upper and lower bounds of your sensors.\
**This can be done by:**
- Measuring the raw value when the moisture sensor is dry,
- Measuring the raw value when the moisture sensor is fully wet, i.e submerged in water.

**These raw values can be seen on the [ESP32 monitor](#using-the-esp32-as-a-moisture-sensor).**

Now open sdkconfig and search for "Wi-Fi Configuration" and edit the following values based
on your Wi-Fi settings.\
```CONFIG_ESP_WIFI_SSID="ESP32"```\
```CONFIG_ESP_WIFI_PASSWORD="12345678"```

**Afterwards enter the commands:**\
```idf.py set-target esp32```\
```idf.py -p <serial-port> flash monitor```\
Note: For help on determining the serial port, follow [this guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/establish-serial-connection.html).

Using the ESP32 as a Moisture Sensor
------------------------------------
After flashing is complete, the ESP32 monitor will pop up in the terminal, and you will be able to see a message such as\
```I (1905) Server: Server running @ 127.0.0.1```\
```I (1905) Server: Connected to AP -> SSID: ESP32, Password: 12345678```\
if everything is done correctly.\
Make sure to take note of the IP, as this will be used to read the exact moisture percentage.

Next, paste the displayed IP into your desired browser, followed by /sensor, so for example\
```127.0.0.1/sensor```\
will display the three sensors, their current moisture level, their optimal moisture level, and hours since they were last watered.
