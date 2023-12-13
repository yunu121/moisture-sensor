/** @file   led.h
    @author Yunu Cho
    @date   7 December 2023
    @brief  Declarations of methods for the LED module.
*/

#ifndef LED_H
#define LED_H

#include "driver/gpio.h"
#include "led_strip.h"

/** Configures LEDs as output.
    @param  gpio_num_t              gpio_num the GPIO port number
    @param  led_strip_config_t      *strip_config pointer to the LED strip configuration
    @param  led_strip_rmt_config_t  *rmt_config pointer to the RMT configuration
    @param  led_strip_handle_t      *led_strip pointer to the LED strip handler
    @return None
*/
void configure_led(gpio_num_t gpio_num, led_strip_config_t *strip_config, led_strip_rmt_config_t *rmt_config, led_strip_handle_t *led_strip);

/** Blinks LED strip while illuminating to the given index.
    @param  int                 index the number of LEDs to be illuminated
    @param  int                 s_led_state the current state of the LED strip, 0 for OFF, 1 for ON
    @param  led_strip_handle_t  led_strip the handler for the LED strip
    @return None
*/
void blink_led(int index, int s_led_state, led_strip_handle_t led_strip);

/** Toggles LED strip while illuminating to the given index.
    @param  int                 index the number of LEDs to be illuminated
    @param  led_strip_handle_t  led_strip the handler for the LED strip
    @return None
*/
void toggle_led(int index, led_strip_handle_t led_strip);

#endif