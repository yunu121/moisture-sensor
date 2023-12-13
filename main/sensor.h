/** @file   sensor.h
    @author Yunu Cho
    @date   7 December 2023
    @brief  Declaration of methods for the moisture sensor.
*/
#ifndef SENSOR_H
#define SENSOR_H
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

void configure_sensor(gpio_num_t adc_pin, adc_channel_t channel, adc_oneshot_unit_init_cfg_t *adc_config, adc_oneshot_chan_cfg_t *config, adc_oneshot_unit_handle_t *sensor);
void read_sensor(adc_oneshot_unit_handle_t sensor, adc_channel_t channel, int *out);
#endif