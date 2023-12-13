/** @file   user_defined.h
    @author Yunu Cho
    @date   13 December 2023
    @brief  Macros for user defined specifications.
    @note   Change these values based on your setup. Refer to datasheet for GPIO and ADC values.
*/

#ifndef USER_DEFINED_H
#define USER_DEFINED_H

/*  GPIO and ADC Setup Macros Start  */
#define SENSOR_PIN_1 GPIO_NUM_36
#define SENSOR_UNIT_1 ADC_UNIT_1
#define SENSOR_CHANNEL_1 ADC_CHANNEL_0

#define SENSOR_PIN_2 0
#define SENSOR_UNIT_2 0
#define SENSOR_CHANNEL_2 0

#define SENSOR_PIN_3 0
#define SENSOR_UNIT_3 0
#define SENSOR_CHANNEL_3 0

#define LED_STRIP_PIN GPIO_NUM_14
/*  GPIO and ADC Setup Macros End  */

/*  Soil Moisture and Sensor Macros Start  */
#define OPTIMAL_MOISTURE 60  // It will probably be a good idea to use plants with similar optimal moisture levels

#define UPPER_BOUND 2990  // Upper bound for raw input, i.e. when it is dry
#define LOWER_BOUND 1990  // Lower bound for raw input, i.e. when it is wet
/*  Soil Moisture and Sensor Macros End  */

#endif