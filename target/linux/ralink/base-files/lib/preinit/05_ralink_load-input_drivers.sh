#
# Copyright (C) 2011 OpenWrt.org
#

preinit_ralink_load_input_drivers() {
	insmod input-core
	insmod input-polldev
	insmod gpio_buttons
	insmod button-hotplug
}

boot_hook_add preinit_main preinit_ralink_load_input_drivers
