/** @file   user_config.h
    @author Yunu Cho
    @date   13 December 2023
    @brief  Macros for user defined specifications.
    @note   Change these values based on your setup. Refer to datasheet for GPIO and ADC values.
*/

#ifndef USER_CONFIG_H
#define USER_CONFIG_H

/*  GPIO and ADC Setup Macros Start  */
/** @note Sensor 1 is enabled by default!  */
#define SENSOR_1
#define SENSOR_PIN_1 GPIO_NUM_36
#define SENSOR_UNIT_1 ADC_UNIT_1
#define SENSOR_CHANNEL_1 ADC_CHANNEL_0

/** @note Uncomment the below line and configure macros if sensor 2 is used  */ 
// #define SENSOR_2
#ifdef SENSOR_2
    #define SENSOR_PIN_2 0
    #define SENSOR_UNIT_2 0
    #define SENSOR_CHANNEL_2 0
#endif

/** @note Uncomment the below line and configure macros if sensor 3 is used.  */ 
// #define SENSOR_3
#ifdef SENSOR_3
    #define SENSOR_PIN_3 0
    #define SENSOR_UNIT_3 0
    #define SENSOR_CHANNEL_3 0
#endif

/** @note Uncomment the below line and configure macros if pump(s) are to be used.
          When PUMP_ENABLED is defined, PUMP_1 is defined automatically.  */ 
// #define PUMP_ENABLED
#ifdef PUMP_ENABLED

    #define FLOW_RATE 0

    #define PUMP_1
    #define PUMP_PIN_1 0
    
    // #define PUMP_2
    #ifdef PUMP_2
        #define PUMP_PIN_2 0
    #endif
    
    // #define PUMP_3
    #ifdef PUMP_3
        #define PUMP_PIN_3 0
    #endif

#endif

#define LED_STRIP_PIN GPIO_NUM_14
#define MAX_LEDS 4 // Number of LEDs on the LED strip
/*  GPIO and ADC Setup Macros End  */

/*  Soil Moisture and Sensor Macros Start  */
/** @note For plant usage, it would be good to use plants with similar optimal soil moistures.  */
#define OPTIMAL_MOISTURE 18  // Optimal moisture for the plant(s)

/** @note Raw ADC input and moisture are inversely proportional!  */ 
#define UPPER_BOUND 2990  // Upper bound for raw input, i.e. when it is dry
#define LOWER_BOUND 1990  // Lower bound for raw input, i.e. when it is wet
/*  Soil Moisture and Sensor Macros End  */

#endif