idf_component_register(SRCS "main.c"
                            "led.c"
                            "sensor.c"
                           
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_driver_gpio 
                                  esp_adc
                                  esp_https_server
                                  esp_wifi
                                  nvs_flash
                                  esp_eth
                    )


