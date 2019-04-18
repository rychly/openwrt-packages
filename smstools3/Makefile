#
# Copyright (C) 2018 Marek Rychly
#
# This is free software, licensed under the GNU General Public License v2.
#
# To enable this package, copy the content of this directory into <openwrt>/package/utils/smstools3
# and run: make -j5 -l4 tools/install toolchain/install package/utils/smstools3/compile
#  -j option sets the maximum number of parallel jobs that can be run via make (#CPUs+1 to help ensure saturation of processor utilization)
#  -l prevents any new parallel job starting unless the load is below the amount specified
# the resulting package will be in directory: <openwrt>/bin/<target>/packages
#

include $(TOPDIR)/rules.mk

PKG_NAME:=smstools3
PKG_VERSION:=3.1.21
PKG_RELEASE:=3

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://smstools3.kekekasvi.com/packages
PKG_HASH:=a26ba4c02b16f6cf13177bffca6c9230dc5fefaeba8e3030cd4e4905f6a92084

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/smstools3
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Sms Server Tools 3
	DEPENDS:=+mm
	URL:=http://smstools3.kekekasvi.com/
endef

define Package/smstools3/description
	The SMS Server Tools 3 is a SMS Gateway software which can send and receive
	short messages through GSM modems and mobile phones.
	Thanks to z-wer for OpenWRT Makefile and patches at
	http://smstools3.kekekasvi.com/topic.php?post=4582#post4582
	Thank to Harald Geyer for OpenWRT distribution patches at
	https://github.com/openwrt/packages/tree/master/utils/smstools3/patches
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC=$(TARGET_CC) \
		USER_CFLAGS='$(TARGET_CFLAGS) $(EXTRA_CFLAGS) \
			$(TARGET_CPPFLAGS) $(EXTRA_CPPFLAGS)' \
		USER_LDFLAGS='$(TARGET_LDFLAGS) $(EXTRA_LDFLAGS)'
endef

define Package/smstools3/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/smsd $(PKG_BUILD_DIR)/scripts/sendsms $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DATA) -m 0755 ./files/smstools3.init $(1)/etc/init.d/smstools3
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DATA) ./files/smstools3.conf $(1)/etc/smsd.conf
endef

$(eval $(call BuildPackage,smstools3))
