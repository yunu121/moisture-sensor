/** @file   pump.h
    @author Yunu Cho
    @date   13 December 2023
    @brief  Methods to configure and drive the water pump.
    @note   Declare more functions if necessary. Only a skeleton module!
*/

#ifndef PUMP_H
#define PUMP_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/** Configures water pump as output.
    @param  gpio the GPIO port to set as output.
    @return None
*/
void configure_pump(uint64_t gpio);

/** Calculates the volume of water needed to reach optimal moisture
    for a given soil volume.
    @param  soil_volume      the volume of soil in a planter
    @param  moisture         the current soil moisture, given as a percentage
    @param  optimal_moisture the optimal soil moisture, given as a percentage
    @return The volume of water needed to pump
*/
float calculate_volume(int soil_volume, float moisture, int optimal_moisture);

/** Calculates time needed to drive the pump for, based on volume needed to pump.
    @param  volume the volume of water needed to pump
    @param  float  flow the flowrate of the pump
    @return The time needed to drive the pump for.
*/
int calculate_duration(float volume, float flow);

/** Drives pump for a given amount of seconds.
    @param  seconds the number of seconds to drive pump for.
    @return None
*/
void drive(uint64_t gpio, int seconds);

#endif