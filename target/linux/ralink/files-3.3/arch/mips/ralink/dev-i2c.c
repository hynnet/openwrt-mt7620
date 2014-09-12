#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/i2c/at24.h>
#include <linux/i2c.h>
#include <asm/mach-ralink/rt_mmap.h>

static struct at24_platform_data at24_config = {
	.byte_len	= 0x400 / 8,
	.page_size	= 8,
};

static struct i2c_board_info i2c_info[] __initconst =  {
	{	
		I2C_BOARD_INFO("24c01", 0x50),
		.platform_data	= &at24_config,
	},
};
static struct resource i2c_resources[] = {
	{
		.start		= -1, /* filled at runtime */
		.end		= -1, /* filled at runtime */
		.flags		= IORESOURCE_MEM,
	},
};

static struct platform_device ralink_i2c_device = {
	.name		= "Ralink-I2C",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(i2c_resources),
	.resource	= i2c_resources,
};

int __init ralink_i2c_register(void)
{
	i2c_resources[0].start = RALINK_I2C_BASE;
	i2c_resources[0].end += RALINK_I2C_BASE + 256 - 1;

	platform_device_register(&ralink_i2c_device);
	i2c_register_board_info(0, i2c_info, ARRAY_SIZE(i2c_info));
	return 0;
}
arch_initcall(ralink_i2c_register);
