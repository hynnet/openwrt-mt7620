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
#include <linux/rtl8367.h>
#include <linux/ethtool.h>

#include "devices.h"


#define RT_N56U_GPIO_LED_POWER		0
#define RT_N56U_GPIO_LED_LAN		19
#define RT_N56U_GPIO_LED_USB		24
#define RT_N56U_GPIO_LED_WAN		27
#define RT_N56U_GPIO_BUTTON_RESET	13
#define RT_N56U_GPIO_BUTTON_WPS		26

#define RT_N56U_GPIO_RTL8367_SCK	2
#define RT_N56U_GPIO_RTL8367_SDA	1

#define RT_N56U_KEYS_POLL_INTERVAL	20
#define RT_N56U_KEYS_DEBOUNCE_INTERVAL	(3 * RT_N56U_KEYS_POLL_INTERVAL)


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

static struct mtd_partition rt_n56u_partitions[]=
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
	  size:0x100000,
//	  mask_flags:MTD_WRITEABLE
	  
	}, {
	  name:"rootfs",
	  offset:0x150000,
	  size:0x800000 - 0x150000,  
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

static struct physmap_flash_data rt_n56u_nor_flash_data[] = {
	{
		.width		= 2,	/* bankwidth in bytes */
		.parts		= rt_n56u_partitions,
		.nr_parts	= ARRAY_SIZE(rt_n56u_partitions)
	}
};

static struct resource rt_n56u_nor_resource = {
	.start	= 0x1c000000,
	.end	= 0x1cffffff,
	.flags	= IORESOURCE_MEM,
};

static struct platform_device rt_n56u_nor = {
	.name	= "physmap-flash",
	.id	= -1,
	.dev	= {
		.platform_data = rt_n56u_nor_flash_data,
	},
	.resource = &rt_n56u_nor_resource,
	.num_resources = 1,
};


static struct gpio_led rt_n56u_leds_gpio[] __initdata = {
	{
		.name		= "asus:blue:power",
		.gpio		= RT_N56U_GPIO_LED_POWER,
		.active_low	= 1,
	},
	{
		.name		= "asus:blue:lan",
		.gpio		= RT_N56U_GPIO_LED_LAN,
		.active_low	= 1,
	},
	{
		.name		= "asus:blue:wan",
		.gpio		= RT_N56U_GPIO_LED_WAN,
		.active_low	= 1,
	},
	{
		.name		= "asus:blue:usb",
		.gpio		= RT_N56U_GPIO_LED_USB,
		.active_low	= 1,
	},
};

static struct gpio_keys_button rt_n56u_gpio_buttons[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = RT_N56U_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= RT_N56U_GPIO_BUTTON_RESET,
		.active_low	= 1,
	},
	{
		.desc		= "wps",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = RT_N56U_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= RT_N56U_GPIO_BUTTON_WPS,
		.active_low	= 1,
	}
};



static struct ralink_gpio_mode rt_n56u_board_gpio_mode = {
	.i2c	= 1,
	.spi	= 1,
	.uartf	= 1,
	.uartl	= 0,
	.jtag	= 1,
	.mdio	= 0,
};

#if 1
static struct rtl8367_extif_config rt_n56u_rtl8367_extif1_cfg = {
	.txdelay = 1,
	.rxdelay = 0,
	.mode = RTL8367_EXTIF_MODE_RGMII,
	.ability = {
		.force_mode = 1,
		.txpause = 1,
		.rxpause = 1,
		.link = 1,
		.duplex = 1,
		.speed = RTL8367_PORT_SPEED_1000,
	}
};

static struct rtl8367_platform_data rt_n56u_rtl8367_data = {
	.gpio_sda	= RT_N56U_GPIO_RTL8367_SDA,
	.gpio_sck	= RT_N56U_GPIO_RTL8367_SCK,
	.extif1_cfg	= &rt_n56u_rtl8367_extif1_cfg,
};

static struct platform_device rt_n56u_rtl8367_device = {
	.name		= RTL8367_DRIVER_NAME,
	.id		= -1,
	.dev = {
		.platform_data	= &rt_n56u_rtl8367_data,
	}
};

#endif

static void __init rt_n56u_init(void)
{
	ralink_gpiomode_init(&rt_n56u_board_gpio_mode);
	platform_device_register(&rt_n56u_nor);
	
	platform_device_register(&rt_n56u_rtl8367_device);
	ralink_register_gpio_leds(-1, ARRAY_SIZE(rt_n56u_leds_gpio),
				  rt_n56u_leds_gpio);

	ralink_register_gpio_buttons(-1, RT_N56U_KEYS_POLL_INTERVAL,
				     ARRAY_SIZE(rt_n56u_gpio_buttons),
				     rt_n56u_gpio_buttons);

}

MIPS_MACHINE(RALINK_MACH_RT_N56U, "RT-N56U", "Asus RT-N56U", rt_n56u_init);

