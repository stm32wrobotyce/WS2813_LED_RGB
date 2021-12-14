/*
 * led_ws2812b.h
 *
 *  Created on: 2 paź 2021
 *      Author: piotr
 */

#ifndef INC_LED_WS2813B_H_
#define INC_LED_WS2813B_H_

#include "main.h"

#define RGB_LED_BUFFER_SIZE		24
#define	NUMBER_OF_LEDS			30
#define HEAD_LEDS_BUFFER_SIZE	(NUMBER_OF_LEDS*RGB_LED_BUFFER_SIZE + 1)

struct __attribute__((packed)) rgb_led
{
	uint8_t green[8];
	uint8_t red[8];
	uint8_t blue[8];
};

union rgb_led_buffer
{
    uint8_t bytes[HEAD_LEDS_BUFFER_SIZE];

    struct __attribute__((packed))
    {
		struct rgb_led leds[NUMBER_OF_LEDS];
		uint8_t stop;
    };
};

void led_init(void);
void led_set_all_led_colors(uint8_t green, uint8_t red, uint8_t blue);
void led_set_one_led_colors(uint32_t led_number, uint8_t green, uint8_t red, uint8_t blue);
void led_set_colors_data(struct rgb_led *led, uint8_t green, uint8_t red, uint8_t blue);
void led_convert_color_data_to_ws2812b_format(uint8_t color_data, uint8_t *buffer);
void led_send_led_colors_to_head(void);

#endif /* INC_LED_WS2813B_H_ */
