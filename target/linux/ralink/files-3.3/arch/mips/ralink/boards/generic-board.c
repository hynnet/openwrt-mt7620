/*
 *  Generic Ralink board
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
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/gpio_keys.h>
#include <linux/input.h>

#include "devices.h"


static void __init ralink_generic_init(void)
{
 	printk("Not Machine Type Select !!!\n");

}


MIPS_MACHINE(RALINK_MACH_GENERIC, "Generic", "Generic Ralink board",
	     ralink_generic_init);
