/** @file   led.c
    @author Yunu Cho
    @date   7 December 2023
    @brief  Implementation of a LED strip.
            Can either toggle or blink.
*/

#include "led.h"

void configure_led(gpio_num_t gpio_num, led_strip_config_t *strip_config, led_strip_rmt_config_t *rmt_config, led_strip_handle_t *led_strip)
{
    strip_config -> strip_gpio_num = gpio_num;
    strip_config -> max_leds = 4;

    rmt_config -> resolution_hz = 10000000;
    rmt_config -> flags.with_dma = 0;

    ESP_ERROR_CHECK(led_strip_new_rmt_device(strip_config, rmt_config, led_strip));
    led_strip_clear(*led_strip);
}

void blink_led(int index, int s_led_state, led_strip_handle_t led_strip)
{
    if (s_led_state) {
        for (int i = 0; i < index; i++) {
            led_strip_set_pixel(led_strip, i, 255, 0, 0);
            led_strip_refresh(led_strip);
        }
    } else {
        led_strip_clear(led_strip);
    }
}

void toggle_led(int index, led_strip_handle_t led_strip) 
{
    for (int i = 0; i < index; i++) {
        led_strip_set_pixel(led_strip, i, 0, 255, 0);
        led_strip_refresh(led_strip);
    }
}