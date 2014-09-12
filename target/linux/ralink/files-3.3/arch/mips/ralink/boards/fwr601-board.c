/*
 *  FeiXun FWR601 board
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
#include <asm/mach-ralink/gpio.h>
#include <linux/platform_device.h>

#include "devices.h"

#define FWR601_GPIO_BUTTON_RESET 10
#define FWR601_KEYS_POLL_INTERVAL 20
#define FWR601_KEYS_DEBOUNCE_INTERVAL	(3 * FWR601_KEYS_POLL_INTERVAL)

static struct gpio_led fwr601_led_pins[] = {
	{
		.name		= "fwr601:wps",
		.gpio		= 14,
		.active_low	= 1,
	},
};

static struct gpio_keys_button fwr601_gpio_buttons[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = FWR601_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= FWR601_GPIO_BUTTON_RESET,
		.active_low	= 1,
	}
};

static struct ralink_gpio_mode fwr601_board_gpio_mode = {
	.i2c	= 1,
	.spi	= 0, /* We use SPI flash,so SPI must not be gpio mode */
	.uartf	= 1,
	.uartl	= 0,
	.jtag	= 1,
	.mdio	= 0,
	.spi_refclk = 0,
};

static void __init fwr601_init(void)
{
	ralink_gpiomode_init(&fwr601_board_gpio_mode);
	ralink_register_gpio_leds(-1, ARRAY_SIZE(fwr601_led_pins),
				  fwr601_led_pins);
	ralink_register_gpio_buttons(-1, FWR601_KEYS_POLL_INTERVAL,
				     ARRAY_SIZE(fwr601_gpio_buttons),
				     fwr601_gpio_buttons);
}
MIPS_MACHINE(RALINK_MACH_FWR601, "FWR601", "FeiXun FWR-601",
	     fwr601_init);