#
# Copyright (C) 2014 Marek Rychly
#
# This is free software, licensed under the GNU General Public License v2.
#
# To enable this package, copy the content of this directory into <openwrt>/package/utils/logtrigger
# and run: make -j5 -l4 tools/install toolchain/install package/utils/logtrigger/compile
#  -j option sets the maximum number of parallel jobs that can be run via make (#CPUs+1 to help ensure saturation of processor utilization)
#  -l prevents any new parallel job starting unless the load is below the amount specified
# the resulting package will be in directory: <openwrt>/bin/<target>/packages
#

include $(TOPDIR)/rules.mk

PKG_NAME:=logtrigger
PKG_VERSION:=1.04
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/logtrigger
PKG_HASH:=5d4eafe758c3cf52f46b2b643b76025906ade5943b7789fafd6d1a6e2628c45d

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/logtrigger
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Logtrigger read each logged event, extract information from it and publishes it in system variables that can be read by an external process.
	URL:=https://code.google.com/archive/p/logtrigger/
	DEPENDS:=+libuci
	MAINTAINER:=Marek Rychly <marek.rychly@gmail.com>
endef

define Package/logtrigger/description
	Logtrigger read each logged event, extract information from it
	and publishes it in system variables that can be read by
	an external process called by Logtrigger.
	This makes it very flexible and has great potential since
	the processes can be written in any programming or
	scripting language which enables you to automate tasks
	in the infinity of time the event happens.
	Sources:
	https://code.google.com/p/x-wrt/source/browse/trunk/package/logtrigger/Makefile
	https://forum.openwrt.org/viewtopic.php?pid=219768#p219768
endef

TARGET_CPPFLAGS := \
	-I$(STAGING_DIR)/usr/include/ \
	-I$(LINUX_DIR)/include \
	-Wall \
	-DOPENWRT \
	$(TARGET_CPPFLAGS)

define Build/Prepare
	$(call Build/Prepare/Default)
endef

define Build/Compile
	CFLAGS="$(TARGET_CPPFLAGS) $(TARGET_CFLAGS)" \
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		OPENWRT=1 \
		LIBS="$(TARGET_LDFLAGS) -lm -luci"
endef

define Package/logtrigger/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/logtrigger
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/conffile/OpenWrt/logtrigger.init $(1)/etc/init.d/logrigger
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/logtrigger $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/conffile/scripts/* $(1)/etc/logtrigger/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/conffile/OpenWrt/logtrigger.conf $(1)/etc/config/logtrigger
endef

$(eval $(call BuildPackage,logtrigger))
