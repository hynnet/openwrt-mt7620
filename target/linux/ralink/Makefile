#
# Copyright (C) 2012 Dreambox Pandora
#
# lintel<lintel.huang@gmail.com>
#
#

include $(TOPDIR)/rules.mk

ARCH:=mipsel
BOARD:=ralink
BOARDNAME:=Ralink SoC
SUBTARGETS:=mt7620
FEATURES:=squashfs
LINUX_VERSION:=3.3.8
CFLAGS:=-Os -pipe -mips32r2 -mtune=mips32r2 -funit-at-a-time
include $(INCLUDE_DIR)/target.mk

# DreamBox Pandora specific ralink kernel drivers, a ralink configuration userspace
# utility and the wireless-tools (since ra_wifi doesn't strictly depend
# on it, but we need it).

DEFAULT_PACKAGES += ralink-wifi \
		    ralink-utils \
		    wireless-tools \
		    maccalc gpioctl kmod-gpio-dev \
		    kmod-leds-gpio kmod-ledtrig-gpio kmod-ledtrig-timer kmod-ledtrig-netdev \
		    kmod-gpio-button-hotplug kmod-input-gpio-keys-polled 
define Target/Description
	Build firmware images for Ralink Soc Boards
endef

$(eval $(call BuildTarget))
