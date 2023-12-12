/** @file   led.h
    @author Yunu Cho
    @date   7 December 2023
    @brief  Declarations of methods for the LED module.
*/
#ifndef LED_H
#define LED_H

#include "driver/gpio.h"
#include "led_strip.h"

void configure_led(gpio_num_t gpio_num, led_strip_config_t *strip_config, led_strip_rmt_config_t *rmt_config, led_strip_handle_t *led_strip);
void blink_led(int index, int s_led_state, led_strip_handle_t led_strip);
void toggle_led(int index, led_strip_handle_t led_strip);

#endif