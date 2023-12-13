/** @file   sensor.c
    @author Yunu Cho
    @date   7 December 2023
    @brief  Methods to configure and read from a moisture sensor.
*/

#include "sensor.h"

/** Configures the sensor.
    @param gpio_num_t adc_pin the pin that is connected to the sensor
    @param adc_channel_t channel the channel of the used pin
    @param adc_oneshot_unit_init_cfg_t *adc_config pointer to the initial ADC configuration
    @param adc_oneshot_chan_cfg_t *config pointer to ADC configuration
    @param adc_oneshot_unit_handle_t *sensor the pointer to the handle for the sensor
    @return None
*/
void configure_sensor(gpio_num_t adc_pin, adc_channel_t channel, adc_oneshot_unit_init_cfg_t *adc_config, adc_oneshot_chan_cfg_t *config, adc_oneshot_unit_handle_t *sensor)
{
    adc_config -> unit_id = ADC_UNIT_1;
    ESP_ERROR_CHECK(adc_oneshot_new_unit(adc_config, sensor));
    
    config -> bitwidth = ADC_BITWIDTH_DEFAULT;
    config -> atten = ADC_ATTEN_DB_12;
    
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*sensor, channel, config));
}

/** Reads from the sensor.
    @param adc_oneshot_unit_handle_t *sensor the pointer to the handle for the sensor
    @param adc_channel_t channel the pin that is connected to the sensor
    @param int *out the pointer to the moisture integer
    @return None
*/
void read_sensor(adc_oneshot_unit_handle_t sensor, adc_channel_t channel, int *out)
{
    adc_oneshot_read(sensor, channel, out);
}