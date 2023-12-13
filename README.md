sensor: An implementation of an automated plant watering system
========================================================================
**sensor** is an automated plant system with an ESP32 with Wi-Fi, 
soil moisture sensor, and an LED strip.

Requirements
------------
In order to build **sensor** you need all the dependencies for the ESP32 by following [this guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/).

**Clone this repository to esp:**\
```cd /Users/<user>/esp```\
```git clone https://github.com/yunu121/sensor.git```

Flashing to ESP32
-----------------
Open sensor.c and adjust values from lines 22-29 depending on your setup.\
Make sure to calibrate the values by testing upper and lower bounds.

Now open sdkconfig and search for "Wi-Fi Configuration" and edit the following values based
on your Wi-Fi settings.
```CONFIG_ESP_WIFI_SSID="ESP32"```\
```CONFIG_ESP_WIFI_PASSWORD="12345678"```

**Afterwards enter the commands:**\
```idf.py set-target esp32```\
```idf.py -p <serial-port> flash monitor```\
Note: For help on determining the serial port, follow [this guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/establish-serial-connection.html).
