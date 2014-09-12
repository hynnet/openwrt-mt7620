/*
 *  Mercury MW305R board
 *
 *  Copyright (C) 2013 lintel <lintel.huang@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */


#include <linux/init.h>
#include <linux/sched.h>

#include <asm/mach-ralink/machine.h>

#include "devices.h"

#define MW305R_GPIO_LED_WIFI_GREEN	72 /* active low */
#define MW305R_GPIO_LED_SYS_GREEN	38 /* active low */

#define MW305R_GPIO_BUTTON_RESET	1 /* active low */

#define MW305R_KEYS_POLL_INTERVAL	20
#define MW305R_KEYS_DEBOUNCE_INTERVAL (3 * MW305R_KEYS_POLL_INTERVAL)

static struct ralink_gpio_mode mw305r_board_gpio_mode = {
	.i2c	= 1,
	.spi	= 0, /* We use SPI flash,so SPI must not be gpio mode */
	.uartf	= 1,
	.uartl	= 0,
	.jtag	= 0,
	.mdio	= 0,
	.spi_refclk = 1,
};

static struct gpio_led mw305r_leds_gpio[] = {
	{
		.name		= "mw305r:sys",
		.gpio		=  MW305R_GPIO_LED_SYS_GREEN,
		.active_low	= 1,
	},{
		.name		= "mw305r:wifi",
		.gpio		= MW305R_GPIO_LED_WIFI_GREEN,
		.active_low	= 1,
	},
};

static struct gpio_keys_button mw305r_gpio_buttons[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = MW305R_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= MW305R_GPIO_BUTTON_RESET,
		.active_low	= 1,
	},
};

static void __init mr305r_init(void)
{
	ralink_gpiomode_init(&mw305r_board_gpio_mode);
	ralink_register_gpio_leds(-1, ARRAY_SIZE(mw305r_leds_gpio),
				  mw305r_leds_gpio);
	ralink_register_gpio_buttons(-1, MW305R_KEYS_POLL_INTERVAL,
				     ARRAY_SIZE(mw305r_gpio_buttons),
				     mw305r_gpio_buttons);
	

}
MIPS_MACHINE(RALINK_MACH_MW305R, "MW305R", "Mercury MW305R",
	     mr305r_init);
