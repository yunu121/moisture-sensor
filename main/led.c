/** @file   led.c
    @author Yunu Cho
    @date   7 December 2023
    @brief  Implementation of a LED strip.
            Can either toggle or blink.
*/

#include "led.h"
#include "user_config.h"

void configure_led(gpio_num_t gpio_num, led_strip_config_t *strip_config, led_strip_rmt_config_t *rmt_config, led_strip_handle_t *led_strip, int max)
{
    strip_config -> strip_gpio_num = gpio_num;
    strip_config -> max_leds = max;

    rmt_config -> resolution_hz = 10000000;
    rmt_config -> flags.with_dma = 0;

    ESP_ERROR_CHECK(led_strip_new_rmt_device(strip_config, rmt_config, led_strip));
    led_strip_clear(*led_strip);
}

void drive_led(led_strip_handle_t led_strip, int *s_led_state, float relative_moisture, float moisture)
{
    if (relative_moisture < 25) {
        blink_led(1, *s_led_state, led_strip);
    } else if (moisture > OPTIMAL_MOISTURE + 10) {
        blink_led(MAX_LEDS, *s_led_state, led_strip);
    } else {   
        int index = relative_moisture / (100 / MAX_LEDS);
        toggle_led(index, led_strip);
    }
    
    *s_led_state = !(*s_led_state);
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
    
    for (int i = MAX_LEDS; i < index; i--) {
        led_strip_set_pixel(led_strip, i, 0, 0, 0); // Makes sure that LED strip isn't both green and red when switching states
    }
}