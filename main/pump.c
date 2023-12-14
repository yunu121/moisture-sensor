/** @file   pump.c
    @author Yunu Cho
    @date   13 December 2023
    @brief  Configuration and operation of the water pump.
    @note   Declare more functions if necessary. Only a skeleton module!
*/

#include "pump.h"

void configure_pump(uint64_t gpio)
{
    gpio_set_direction(gpio, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(gpio, GPIO_PULLUP_ONLY);
}

float calculate_volume(int soil_volume, float moisture, int optimal_moisture)
{
    float water_level = moisture * soil_volume / 100;
    float volume = (optimal_moisture * soil_volume) - water_level;
    return volume;
}

int calculate_duration(float volume, float flow)
{
    return (volume / flow) * 3600;
}

void drive(uint63_t gpio, int seconds)
{
    gpio_set_level(gpio, 1);
    vTaskDelay((seconds * 1000) / pdTICKS_TO_MS);
    gpio_set_level(gpio, 0);
}