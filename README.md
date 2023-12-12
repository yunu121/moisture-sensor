sensor: An implementation of an automated plant watering system
========================================================================
sensor is an automated plant system with an ESP32 with Wi-Fi, 
soil moisture sensor, and an LED strip.

Requirements
------------
In order to build moisture_sensor you need all the dependencies by following [this guide] (https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/).

Clone this repository to esp:__
```cd /user/esp```__
```git clone <https://github.com/yunu121/sensor.git>```

Flashing to ESP32
-----------------
Open sensor.c and adjust values from lines 22-29 depending on your setup.__
Make sure to calibrate the values by testing upper and lower bounds.__

Now open sdkconfig and search for "Wi-Fi Configuration" and edit these values based
on your Wi-Fi settings.__

Afterwards enter the commands:__
```idf.py set-target esp32```__
```idf.py -p serial-port flash monitor```__
Note: For help on determining the serial port, follow [this guide] (https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/establish-serial-connection.html).
