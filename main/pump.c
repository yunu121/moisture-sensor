/** @file   pump.c
    @author Yunu Cho
    @date   13 December 2023
    @brief  Configuration and operation of the water pump.
    @note   Declare more functions if necessary. Only a skeleton module!
*/

#include "pump.h"

void configure_pump(void)
{
    ;;
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

void drive(int seconds)
{
    
}