/** @file   pump.h
    @author Yunu Cho
    @date   13 December 2023
    @brief  Methods to configure and drive the water pump.
*/

#ifndef PUMP_H
#define PUMP_H

#include "driver/gpio.h"

/** Configures water pump as output.
    @return None
*/
void configure_pump(void);

/** Drives pump for a given amount of seconds
    @param  int seconds the number of seconds to drive pump for.
    @return None
*/
void drive(int seconds);

#endif