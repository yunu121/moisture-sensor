/** @file   sensor.h
    @author Yunu Cho
    @date   7 December 2023
    @brief  Declaration of methods for the moisture sensor.
*/

#ifndef SENSOR_H
#define SENSOR_H

#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

/** Configures the sensor.
    @param  adc_pin     the pin that is connected to the sensor
    @param  channel     the channel of the used pin
    @param  *adc_config pointer to initial ADC configuration
    @param  *config     pointer to ADC configuration
    @param  *sensor     the pointer to the handle for the sensor
    @return None
*/
void configure_sensor(gpio_num_t adc_pin, adc_channel_t channel, adc_oneshot_unit_init_cfg_t *adc_config, adc_oneshot_chan_cfg_t *config, adc_oneshot_unit_handle_t *sensor);

/** Reads from the sensor.
    @param  *sensor the pointer to the handle for the sensor
    @param  channel the pin that is connected to the sensor
    @param  *out the pointer to the moisture integer, i.e. the variable that the reading will be stored in
    @return None
*/
void read_sensor(adc_oneshot_unit_handle_t sensor, adc_channel_t channel, int *out);

#endif