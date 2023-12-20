/** @file   user_config.h
    @author Yunu Cho
    @date   13 December 2023
    @brief  Macros for user defined specifications.
    @note   Change these values based on your setup. Refer to datasheet for GPIO and ADC values.
*/

#ifndef USER_CONFIG_H
#define USER_CONFIG_H

/*  Time Measurements Conversion Start '*/

/** @note Change the value below to adjust how often plants are watered.
          They will be watered if the said plant is dry, and it has been
          over said time since the plant was last watered. This value may vary
          for each plant, but it will probably be good to keep plants with similar
          watering periods and optimal moisture.  */
#define WATERING_PERIOD 1

/** @note Use this macro to convert hours to seconds. 
          Used when checking if plants need to be watered.  */
#define HOURS_TO_S(X) (X * 3600)

/*  Time Measurements Conversion End  */

/*  GPIO and ADC Setup Macros Start  */

/** @note Use ADC1 for sensor pins! ADC2 is used by the Wi-Fi driver.  */

#define MAX_LEDS 4 // Number of LEDs on the LED strip

/** @note Sensor 1 is enabled by default! Modify the given values depending on which pins are used.  */
#define SENSOR_1

#define SENSOR_PIN_1 GPIO_NUM_36
#define SENSOR_UNIT_1 ADC_UNIT_1
#define SENSOR_CHANNEL_1 ADC_CHANNEL_0
#define LED_STRIP_PIN_1 GPIO_NUM_14

/** @note Uncomment the below line and configure macros if sensor 2 is used.  */ 
#define SENSOR_2

#ifdef SENSOR_2
    #define SENSOR_PIN_2 GPIO_NUM_39
    #define SENSOR_UNIT_2 ADC_UNIT_1
    #define SENSOR_CHANNEL_2 ADC_CHANNEL_3
    #define LED_STRIP_PIN_2 GPIO_NUM_27
#endif

/** @note Uncomment the below line and configure macros if sensor 3 is used.  */ 
#define SENSOR_3

#ifdef SENSOR_3
    #define SENSOR_PIN_3 GPIO_NUM_34
    #define SENSOR_UNIT_3 ADC_UNIT_1
    #define SENSOR_CHANNEL_3 ADC_CHANNEL_6
    #define LED_STRIP_PIN_3 GPIO_NUM_26
#endif

/*  Pump Related Macros Start -- edit and add macros here  */

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

/*  Pump Related Macros End  */

/*  GPIO and ADC Setup Macros End  */

/*  Soil Moisture and Sensor Macros Start  */

/** @note For plant usage, it would be good to use plants with similar optimal soil moistures.  */
#define OPTIMAL_MOISTURE 18  // Optimal moisture for the plant(s)

/** @note Raw ADC input and moisture are inversely proportional!  */ 
#define UPPER_BOUND_1 3020  // Upper bound for raw input, i.e. when it is dry
#define LOWER_BOUND_1 2008  // Lower bound for raw input, i.e. when it is wet

#ifdef SENSOR_2
    #define UPPER_BOUND_2 3020
    #define LOWER_BOUND_2 2008
#endif

#ifdef SENSOR_3
    #define UPPER_BOUND_3 3020
    #define LOWER_BOUND_3 2008
#endif

/*  Soil Moisture and Sensor Macros End  */

#endif