#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/PBX20
	NAME:=PBX20-Board
	PACKAGES:=\
		kmod-usb-core kmod-usb2 \
		kmod-ledtrig-usbdev
endef

define Profile/PBX20/Description
	Default package for PBX20 boards.
endef
$(eval $(call Profile,PBX20))
