# Copyright (C) 2012 DreamBox
# All rights reserved.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# lintel<lintel.huang@gmail.com>
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ralink-utils
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

define Package/ralink-utils
  SECTION:=ralink
  CATEGORY:=Ralink
  TITLE:=Ralink RT305x apps
endef

define Package/ralink-utils/description
	Ralink RT305x appsi
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CONFIGURE_OPTS) CFLAGS="$(TARGET_CFLAGS) -I$(STAGING_DIR)/usr/include -I$(LINUX_DIR)/include  \
	-I$(LINUX_DIR)/arch/mips/include -I$(LINUX_DIR)/drivers/char  -I$(LINUX_DIR)/drivers/net/ethernet/raeth \
	-I$(LINUX_DIR)/drivers/net/ethernet/rt_rdm -I$(LINUX_DIR)/drivers/char/i2s $(EXTRA_KCONFIG) " LDFLAGS="$(TARGET_LDFLAGS)" \
		$(MAKE) -C $(PKG_BUILD_DIR)
endef

define Package/ralink-utils/install
	$(INSTALL_DIR) $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/switch $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ated $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/reg $(1)/sbin/	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mmap $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pciephy  $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mii_mgr $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/i2ccmd $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/i2scmd $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/spicmd $(1)/sbin/
endef

$(eval $(call BuildPackage,ralink-utils))

