/*
 *  D-Link DIR-620 B2
 *
 *  Copyright (C) 2013 lintel <lintel.huang@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <asm/mach-ralink/machine.h>

#include "devices.h"

#define DIR_620_B2_GPIO_LED_WIFI_GREEN	72 /* active low */
#define DIR_620_B2_GPIO_LED_WPS_GREEN	39 /* active low */
#define DIR_620_B2_GPIO_LED_USB_GREEN	38 /* active low */

#define DIR_620_B2_GPIO_BUTTON_RESET	1 /* active low */
#define DIR_620_B2_GPIO_BUTTON_WPS	2  /* active low */

#define DIR_620_B2_KEYS_POLL_INTERVAL	20
#define DIR_620_B2_KEYS_DEBOUNCE_INTERVAL (3 * DIR_620_B2_KEYS_POLL_INTERVAL)

static struct gpio_led dir_620_b2_leds_gpio[] __initdata = {
	{
		.name		= "d-link:green:wps",
		.gpio		= DIR_620_B2_GPIO_LED_WPS_GREEN,
		.active_low	= 0,  /* not active_low */
	}, {
		.name		= "d-link:green:wifi",
		.gpio		= DIR_620_B2_GPIO_LED_WIFI_GREEN,
		.active_low	= 1,
	}, {
		.name		= "d-link:green:usb",
		.gpio		= DIR_620_B2_GPIO_LED_USB_GREEN,
		.active_low	= 1,
	}
};

static struct gpio_keys_button dir_620_b2_gpio_buttons[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = DIR_620_B2_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= DIR_620_B2_GPIO_BUTTON_RESET,
		.active_low	= 1,
	}, {
		.desc		= "wps",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = DIR_620_B2_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= DIR_620_B2_GPIO_BUTTON_WPS,
		.active_low	= 1,
	}
};

static struct ralink_gpio_mode dir_620_b2_board_gpio_mode = {
	.i2c	= 1,
	.spi	= 0, /* We use SPI flash,so SPI must not be gpio mode */
	.uartf	= 1,
	.uartl	= 0,
	.jtag	= 1,
	.mdio	= 0,
	.spi_refclk = 1,
};


static void __init dir_620_b2_init(void)
{
	ralink_gpiomode_init(&dir_620_b2_board_gpio_mode);
	
	ralink_register_gpio_leds(-1, ARRAY_SIZE(dir_620_b2_leds_gpio),
				  dir_620_b2_leds_gpio);
	ralink_register_gpio_buttons(-1, DIR_620_B2_KEYS_POLL_INTERVAL,
				     ARRAY_SIZE(dir_620_b2_gpio_buttons),
				     dir_620_b2_gpio_buttons);
}
MIPS_MACHINE(RALINK_MACH_DIR_620_B2, "DIR-620-B2", "D-Link DIR-620 B2",
	     dir_620_b2_init);
