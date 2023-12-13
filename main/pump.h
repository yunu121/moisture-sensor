/** @file   pump.h
    @author Yunu Cho
    @date   13 December 2023
    @brief  Methods to configure and drive the water pump.
*/

void configure_pump(void);
void drive(int seconds);

#ifndef PUMP_H
#define PUMP_H
#include "driver/gpio.h"
#endif