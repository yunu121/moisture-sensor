idf_component_register(SRCS "sensor.c"
                            "led.c"
                           
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_driver_gpio 
                                  esp_adc
                                  esp_https_server
                                  esp_wifi
                                  nvs_flash
                                  esp_eth
                    )


