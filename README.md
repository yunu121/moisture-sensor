sensor: An implementation of an automated plant watering system
========================================================================
**sensor** is an automated plant system with an ESP32 with Wi-Fi, 
soil moisture sensors, and an LED strip, using ESP-IDF.

Requirements
------------
In order to build **sensor** you need all the dependencies for the ESP32 by following [this guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/).

**Clone this repository to your esp directory:**\
```cd /Users/<user>/esp```\
```git clone https://github.com/yunu121/sensor.git```

Preparations Before Flashing
----------------------------
**Open [user_defined.h](https://github.com/yunu121/sensor/blob/main/main/user_defined.h) and adjust macros depending on your setup.**
- Refer to the [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf) to determine the correct GPIO pins and ADC channels for your exact specifications.

Now open sdkconfig and search for "Wi-Fi Configuration" and edit the following values based
on your Wi-Fi settings.\
```CONFIG_ESP_WIFI_SSID="ESP32"```\
```CONFIG_ESP_WIFI_PASSWORD="12345678"```

**To flash, enter the commands:**\
```idf.py set-target esp32```\
```idf.py build```\
```idf.py -p <serial-port> flash monitor```

Note: For help on determining the serial port, follow [this guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/establish-serial-connection.html).

After First-Time Flash
----------------------
After running the commands shown in the above section, you will be able to see the ESP32 monitor in your terminal.\
This is particularly useful as you will be able to calibrate the sensors based on the raw input displayed.
Make sure to calibrate the values by testing and recording both upper and lower bounds of your sensors, in [user_defined.h](https://github.com/yunu121/sensor/blob/main/main/user_defined.h).

**This can be done by:**
- Measuring the raw value when the moisture sensor is dry,
- Measuring the raw value when the moisture sensor is fully wet, i.e submerged in water.

**Then flash again, for a second time.**

```idf.py -p <serial-port> flash monitor```

Using the ESP32 as a Moisture Sensor
------------------------------------
After the second flash is complete, the ESP32 monitor will pop up in the terminal, and if everything is done correctly, you will be able to see messages such as\

```I (1905) Server: Server running @ 127.0.0.1```\
```I (1905) Server: Connected to AP -> SSID: ESP32, Password: 12345678```

Otherwise, something has gone wrong, and you might want to check that [Wi-Fi Configuration](#after-first-time-flash)
was configured properly (remember everything is case sensitive)!

**Make sure to take note of the IP, as this will be used to access the HTTP server for the plants.**

Next, paste the displayed IP into your desired browser, followed by /sensor, so for example

```127.0.0.1/sensor```

will display the three sensors, their current moisture level, their optimal moisture level, and hours since they were last watered.

**The LED strip displays the plant's current moisture level:**
- A **single flashing LED in RED** indicates that the moisture level is below 25% of its optimal moisture so needs to be watered;
- A **constantly illuminated LED in GREEN** indicates that the moisture level is above 25% of its optimal moisture;
  - * *The number of LEDS illuminated displays the current moisture relative to its optimal moisture.* *
- A **full strip of LEDS flashing in RED** indicates that the current moisture level is above the optimal moisture, and should not be watered again (until moisture goes back down to below optimal).

Author
------
Yunu Cho
