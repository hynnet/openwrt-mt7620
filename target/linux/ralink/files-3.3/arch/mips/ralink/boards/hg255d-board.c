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


#include "devices.h"

/*
 Note:
 GPIO [6:3] is share with SPI interface,so,if use for gpio,must disable the SPI.
 see datasheet Page NO.37.
 */
#define HG255D_GPIO_BUTTON_RESET	4 
#define HG255D_GPIO_BUTTON_WPS		10
#define HG255D_GPIO_BUTTON_WLAN		0

#define HG255D_GPIO_LED_POWER		8
#define HG255D_GPIO_LED_USB		9
#define HG255D_GPIO_LED_INTERNET	13
#define HG255D_GPIO_LED_WLAN		14
#define HG255D_GPIO_LED_WPS		12
#define HG255D_GPIO_LED_VOICE		5


#define HG255D_BUTTONS_POLL_INTERVAL	10



#define SECTOR_64K_SIZE	0x10000
#define SECTOR_128K_SIZE	0x20000
#define FLASH_SIZE		16*1024*1024
#define FLASH_SECTOR_SIZE	 SECTOR_128K_SIZE

#define UBOOT_SIZE	FLASH_SECTOR_SIZE * 2

#define UBOOT_ENV_OFFSET UBOOT_SIZE
#define UBOOT_ENV_SIZE	FLASH_SECTOR_SIZE * 1

#define FACTORY_OFFSET	(UBOOT_ENV_OFFSET + UBOOT_ENV_SIZE)
#define FACTORY_SIZE	FLASH_SECTOR_SIZE * 1

#define KERNEL_OFFSET	(FACTORY_OFFSET + FACTORY_SIZE)
#define KERNEL_SIZE	(SECTOR_128K_SIZE * 7)

#define ROOTFS_OFFSET	(KERNEL_OFFSET + KERNEL_SIZE)
#define ROOTFS_SIZE	(FLASH_SIZE - ROOTFS_OFFSET)

#define LINUX_OFFSET	(KERNEL_OFFSET)
#define LINUX_SIZE	(FLASH_SIZE - KERNEL_OFFSET)

static struct mtd_partition generic_8m_partitions[] =
{
  	/* The following partitions are board dependent. */
	{
	  name:"U-Boot",
	  offset:0,
	  size:0x30000,
	  mask_flags:MTD_WRITEABLE
	}, {
	  name:"U-Boot-Env",
	  offset:0x30000,
	  size:0x10000,
//	  mask_flags:MTD_WRITEABLE
	}, {
	  name:"Factory",
	  offset:0x40000,
	  size:0x10000,
//	  mask_flags:MTD_WRITEABLE	  
	}, {
	  name:"kernel",
	  offset:0x50000,
	  size:0xe0000,
//	  mask_flags:MTD_WRITEABLE
	  
	}, {
	  name:"rootfs",
	  offset:0x130000,
	  size:0x800000 - 0x130000,  
//	  mask_flags:MTD_WRITEABLE
	}, {
	  name:"linux",
	  offset:0x50000,
	  size:0x800000 - 0x50000,
//	  mask_flags:MTD_WRITEABLE
	}, {
	  name:"fullflash",
	  offset:0,
	  size:0,
//	  mask_flags:MTD_WRITEABLE
	}


};

static struct physmap_flash_data hg255d_nor_flash_data[] = {
	{
		.width		= 2,	/* bankwidth in bytes */
		.parts		= generic_8m_partitions,
		.nr_parts	= ARRAY_SIZE(generic_8m_partitions)
	}
};

static struct resource hg255d_nor_resource = {
	.start	= 0xbf000000,
	.end	= 0xbfffffff,
	.flags	= IORESOURCE_MEM,
};

static struct platform_device hg255d_nor = {
	.name	= "physmap-flash",
	.id	= -1,
	.dev	= {
		.platform_data = hg255d_nor_flash_data,
	},
	.resource = &hg255d_nor_resource,
	.num_resources = 1,
};

static struct gpio_led hg255d_led_pins[] = {
   	{
		.name		= "hg255d:power",
		.gpio		= HG255D_GPIO_LED_POWER,
		.active_low	= 1,
		.default_trigger= "default-on",
	}, {
		.name		= "hg255d:internet",
		.gpio		= HG255D_GPIO_LED_INTERNET,
		.active_low	= 1,
	}, {
		.name		= "hg255d:wlan",
		.gpio		= HG255D_GPIO_LED_WLAN,
		.active_low	= 1,
	}, {
		.name		= "hg255d:usb",
		.gpio		= HG255D_GPIO_LED_USB,
		.active_low	= 1,
	}, {
		.name		= "hg255d:wps",
		.gpio		= HG255D_GPIO_LED_WPS,
		.active_low	= 1,
	}, {
		.name		= "hg255d:voice",
		.gpio		= HG255D_GPIO_LED_VOICE,
		.active_low	= 1,
	}
};

#if 1

static struct gpio_keys_button hg255d_gpio_buttons[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = HG255D_BUTTONS_POLL_INTERVAL,
		.gpio		= HG255D_GPIO_BUTTON_RESET,
		.active_low	= 1,
	}, {
		.desc		= "wps",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = HG255D_BUTTONS_POLL_INTERVAL,
		.gpio		= HG255D_GPIO_LED_WPS,
		.active_low	= 1,
	}, {
		.desc		= "wlan",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = HG255D_BUTTONS_POLL_INTERVAL,
		.gpio		= HG255D_GPIO_BUTTON_WLAN,
		.active_low	= 1,
	}
};


#endif

static struct ralink_gpio_mode hg255d_board_gpio_mode = {
	.i2c	= 1,
	.spi	= 1,
	.uartf	= 1,
	.uartl	= 0,
	.jtag	= 1,
	.mdio	= 0,
};


static void __init hg255d_init(void)
{
	ralink_gpiomode_init(&hg255d_board_gpio_mode);
	platform_device_register(&hg255d_nor);
	ralink_register_gpio_leds(-1, ARRAY_SIZE( hg255d_led_pins),
				   hg255d_led_pins);
	ralink_register_gpio_buttons(-1, HG255D_BUTTONS_POLL_INTERVAL,
                              ARRAY_SIZE(hg255d_gpio_buttons),
			      hg255d_gpio_buttons);

}

MIPS_MACHINE(RALINK_MACH_HG255D, "HG255D", "HuaWei HG255D",
	     hg255d_init);
