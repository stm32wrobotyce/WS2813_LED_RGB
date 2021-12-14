/*
 * led_ws2812b.c
 *
 *  Created on: 2 paź 2021
 *      Author: piotr
 */
#include "led_ws2813b.h"
#include "main.h"
#include "tim.h"

#define LED_COMM_HIGH_STATE		64
#define LED_COMM_LOW_STATE		24
#define LED_COMM_RESET_STATE	0

#define BIT_IN_BYTES			8

union rgb_led_buffer leds_buffer;

const uint8_t gamma_correction[] = {
	    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
	    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
	    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
	    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
		10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
		17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
		25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
		37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
		51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
		69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
		90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
		115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
		144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
		177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
		215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

void led_init(void)
{
	uint32_t i;

	for(i=0; i < NUMBER_OF_LEDS; i++)
	{
		led_set_one_led_colors(i, 0, 0, 0);
	}

	leds_buffer.stop = LED_COMM_RESET_STATE;
}

void led_set_all_led_colors(uint8_t green, uint8_t red, uint8_t blue)
{
	uint32_t i;

	for(i=0; i < NUMBER_OF_LEDS; i++)
	{
		led_set_one_led_colors(i, green, red, blue);
	}
}

void led_set_one_led_colors(uint32_t led_number, uint8_t green, uint8_t red, uint8_t blue)
{
	led_set_colors_data(&(leds_buffer.leds[led_number]), green, red, blue);
}

void led_set_colors_data(struct rgb_led *led, uint8_t green, uint8_t red, uint8_t blue)
{
	led_convert_color_data_to_ws2812b_format(gamma_correction[green], led->green);
	led_convert_color_data_to_ws2812b_format(gamma_correction[red], led->red);
	led_convert_color_data_to_ws2812b_format(gamma_correction[blue], led->blue);
}

void led_convert_color_data_to_ws2812b_format(uint8_t color_data, uint8_t *buffer)
{
	for(uint8_t i=0; i<BIT_IN_BYTES; i++)
	{
		if(((color_data >> i) & 0x01) == 1)
		{
			*(buffer+(BIT_IN_BYTES-i-1)) = LED_COMM_HIGH_STATE;
		}
		else
		{
			*(buffer+(BIT_IN_BYTES-i-1)) = LED_COMM_LOW_STATE;
		}
	}
}

void led_send_led_colors_to_head(void)
{
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)leds_buffer.bytes, HEAD_LEDS_BUFFER_SIZE);
}
