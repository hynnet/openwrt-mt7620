/*
 *  Mercury WR8305RT board
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

#define WR8305RT_GPIO_LED_WIFI	72 /* active low */
#define WR8305RT_GPIO_LED_SYS	38 /* active low */
#define WR8305RT_GPIO_LED_USB	39 /* active low */

#define WR8305RT_GPIO_BUTTON_RESET	1 /* active low */

#define WR8305RT_KEYS_POLL_INTERVAL	20
#define WR8305RT_KEYS_DEBOUNCE_INTERVAL (3 * WR8305RT_KEYS_POLL_INTERVAL)

static struct ralink_gpio_mode wr8305rt_board_gpio_mode = {
	.i2c	= 1,
	.spi	= 0, /* We use SPI flash,so SPI must not be gpio mode */
	.uartf	= 1,
	.uartl	= 0,
	.jtag	= 0,
	.mdio	= 0,
	.spi_refclk = 1,
};

static struct gpio_led wr8305rt_leds_gpio[] = {
	{
		.name		= "wr8305rt:sys",
		.gpio		=  WR8305RT_GPIO_LED_SYS,
		.active_low	= 1,
	},{
		.name		= "wr8305rt:wifi",
		.gpio		= WR8305RT_GPIO_LED_WIFI,
		.active_low	= 1,
	},{
		.name		= "wr8305rt:usb",
		.gpio		= WR8305RT_GPIO_LED_USB,
		.active_low	= 0,
	},
};

static struct gpio_keys_button wr8305rt_gpio_buttons[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = WR8305RT_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= WR8305RT_GPIO_BUTTON_RESET,
		.active_low	= 1,
	},
};

static void __init mr305r_init(void)
{
	ralink_gpiomode_init(&wr8305rt_board_gpio_mode);
	ralink_register_gpio_leds(-1, ARRAY_SIZE(wr8305rt_leds_gpio),
				  wr8305rt_leds_gpio);
	ralink_register_gpio_buttons(-1, WR8305RT_KEYS_POLL_INTERVAL,
				     ARRAY_SIZE(wr8305rt_gpio_buttons),
				     wr8305rt_gpio_buttons);
	

}
MIPS_MACHINE(RALINK_MACH_WR8305RT, "WR8305RT", "ZBT WR8305RT",
	     mr305r_init);
