/** @file   sensor.h
    @author Yunu Cho
    @date   7 December 2023
    @brief  Declaration of methods for the moisture sensor.
*/

#ifndef SENSOR_H
#define SENSOR_H

#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

/** Configures the ADC unit.
    @param  unit       the ADC unit for the sensor(s)
    @param  adc_config pointer to initial ADC configuration
    @param  sensor     the pointer to the handle for the ADC unit
    @return None
*/
void configure_unit(adc_unit_t unit, adc_oneshot_unit_init_cfg_t *adc_config, adc_oneshot_unit_handle_t *sensor);

/** Configures the ADC channel.
    @param  channel the ADC channel for the sensor
    @param  sensor  the pointer to the handle for the ADC unit
    @param  config  the  pointer to the channel config for the sensor
    @return None
*/
void configure_channel(adc_channel_t channel, adc_oneshot_unit_handle_t *sensor, adc_oneshot_chan_cfg_t *config);

/** Reads from the sensor.
    @param  sensor  the pointer to the handle for the sensor
    @param  channel the pin that is connected to the sensor
    @param  out     the pointer to the moisture integer, i.e. the variable that the reading will be stored in
    @return None
*/
void read_sensor(adc_oneshot_unit_handle_t sensor, adc_channel_t channel, int *out);

#endif