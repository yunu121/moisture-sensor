/** @file   user_defined.h
    @author Yunu Cho
    @date   13 December 2023
    @brief  Macros for user defined specifications.
*/

#ifndef USER_DEFINED_H
#define USER_DEFINED_H

/** USER DEFINED VALUES START */
//  Change these values based on your setup. Refer to datasheet.
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

#define OPTIMAL_MOISTURE 60

#define UPPER_BOUND 2990  //Upper bound for raw input, i.e. when it is dry
#define LOWER_BOUND 1990  //Lower bound for raw input, i.e. when it is wet
/** USER DEFINED VALUES END */

#endif