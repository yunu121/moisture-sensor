/** @file   sensor.c
    @author Yunu Cho
    @date   7 December 2023
    @brief  Methods to configure and read from a moisture sensor.
*/

#include "sensor.h"

void configure_unit(adc_unit_t unit, adc_oneshot_unit_init_cfg_t *adc_config, adc_oneshot_unit_handle_t *sensor)
{
    adc_config -> unit_id = unit;
    
    ESP_ERROR_CHECK(adc_oneshot_new_unit(adc_config, sensor));
}

void configure_channel(adc_channel_t channel, adc_oneshot_unit_handle_t *sensor, adc_oneshot_chan_cfg_t *config)
{
    config -> bitwidth = ADC_BITWIDTH_DEFAULT;
    config -> atten = ADC_ATTEN_DB_12;
    
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*sensor, channel, config));
}

void read_sensor(adc_oneshot_unit_handle_t sensor, adc_channel_t channel, int *out)
{
    adc_oneshot_read(sensor, channel, out);
}