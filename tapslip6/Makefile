#
# Copyright (C) 2013 Marek Rychly
#
# This is free software, licensed under the GNU General Public License v2.
#
# To enable this package, copy the content of this directory into <openwrt>/package/network/utils/tapslip6
# and run: make -j5 -l4 tools/install toolchain/install package/network/utils/tapslip6/compile
#  -j option sets the maximum number of parallel jobs that can be run via make (#CPUs+1 to help ensure saturation of processor utilization)
#  -l prevents any new parallel job starting unless the load is below the amount specified
# the resulting package will be in directory: <openwrt>/bin/<target>/packages
#

include $(TOPDIR)/rules.mk

PKG_NAME:=tapslip6
PKG_VERSION:=git944537fccf
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME).c
PKG_SOURCE_URL:=https://github.com/contiki-os/contiki/raw/master/tools/stm32w
UNPACK_CMD=cp -v $(DL_DIR)/$(PKG_SOURCE) $(1)/

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/tapslip6
	SECTION:=net
	CATEGORY:=Network
	TITLE:=A driver for uip6-bridge customized for STM32W
	DEPENDS:=+kmod-usb-serial-ftdi +kmod-tun
	URL:=https://github.com/contiki-os/contiki/blob/master/tools/stm32w/tapslip6.c
	MAINTAINER:=Marek Rychly <marek.rychly@gmail.com>
endef

define Package/tapslip6/description
	This package contains the driver for the uip6-bridge customized for STM32W.
	Thanks to Salvatore Pitrulli from Contiki and Adam Dunkels,
	Joakim Eriksson, Niclas Finne for the original code.
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -Wall \
		-o $(PKG_BUILD_DIR)/tapslip6 $(PKG_BUILD_DIR)/tapslip6.c
endef

define Package/tapslip6/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tapslip6 $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,tapslip6))
