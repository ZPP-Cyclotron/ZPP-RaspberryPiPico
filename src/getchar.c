/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * source: https://forums.raspberrypi.com/viewtopic.php?t=303964
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#define ENDSTDIN	255
#define CR		13

int main() {
    stdio_init_all();

    bool onoff = 0;
    const uint LED_PIN = 25;
    char strg[100];
    char chr;
    int lp = 0;

    memset(strg, 0, sizeof(strg));

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {

        chr = getchar_timeout_us(0);
        while(chr != ENDSTDIN)
        {
            strg[lp++] = chr;
            if(chr == CR || lp == (sizeof(strg) - 1))
            {
                strg[lp] = 0;	//terminate string
                printf("You wrote - %s\n", strg);
                //memset(strg, 0, sizeof(strg));
                lp = 0;		//reset string buffer pointer
                break;
            }

            chr = getchar_timeout_us(0);
        }

        gpio_put(LED_PIN, onoff);
        onoff = !onoff;
        sleep_ms(100);
    }
    return 0;
}