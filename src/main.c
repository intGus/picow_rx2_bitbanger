// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2023 Brian Starkey <stark3y@gmail.com>

#include <stdio.h>
#include <string.h>

#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "bt_hid.h"


#define SIGNAL_PIN 14
#define MUSIC_PIN 16
#define BUTTON_PIN 0

void main(void) {
	stdio_init_all();

	gpio_init(SIGNAL_PIN);
	gpio_init(MUSIC_PIN);
	gpio_init(BUTTON_PIN);

	gpio_set_dir(SIGNAL_PIN, GPIO_OUT);
	gpio_set_dir(MUSIC_PIN, GPIO_OUT);
	gpio_set_dir(BUTTON_PIN, GPIO_IN);
	gpio_pull_up(BUTTON_PIN);

	sleep_ms(1000);
	printf("Hello\n");

	multicore_launch_core1(bt_main);
	// Wait for init (should do a handshake with the fifo here?)
	sleep_ms(1000);

	struct bt_hid_state state;
	for ( ;; ) {
		sleep_ms(20);
		bt_hid_get_latest(&state);
		// this part is only related to the RX2 board to control the RC car
		if (state.hat == 1) {
			for(int i=0; i<=3; i++) {
				gpio_put(SIGNAL_PIN, 1);
				sleep_us(1500);

				gpio_put(SIGNAL_PIN, 0);
				sleep_us(500);
			}
			for(int i=0; i<=9; i++) {
				gpio_put(SIGNAL_PIN, 1);
				sleep_us(500);

				gpio_put(SIGNAL_PIN, 0);
				sleep_us(500);
			}
		} else if (state.hat == 5) {
			for(int i=0; i<=3; i++) {
				gpio_put(SIGNAL_PIN, 1);
				sleep_us(1500);

				gpio_put(SIGNAL_PIN, 0);
				sleep_us(500);
			}
			for(int i=0; i<=39; i++) {
				gpio_put(SIGNAL_PIN, 1);
				sleep_us(500);

				gpio_put(SIGNAL_PIN, 0);
				sleep_us(500);
			}
			} else {
			gpio_put(SIGNAL_PIN, 0);
		}

		if (state.buttons == 4 || !gpio_get(BUTTON_PIN)) { 
			gpio_put(MUSIC_PIN, 1);
		} else {
			gpio_put(MUSIC_PIN, 0);
		}


		// printf("buttons: %d, l: %d,%d, r: %d,%d, l2,r2: %d,%d hat: %d\n",
		// 		state.buttons, state.lx, state.ly, state.rx, state.ry,
		// 		state.l2, state.r2, state.hat);

	}
}
