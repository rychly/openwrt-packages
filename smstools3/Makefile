#
# Copyright (C) 2013 Marek Rychly
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
	DEPENDS:=+libiconv-full +iconv +mm
	URL:=http://smstools3.kekekasvi.com/
endef

define Package/smstools3/description
	The SMS Server Tools 3 is a SMS Gateway software which can send and receive
	short messages through GSM modems and mobile phones.
	Thanks to z-wer for OpenWRT Makefile and patches at
	http://smstools3.kekekasvi.com/topic.php?post=4582#post4582
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	sed -i -e 's/^\(.*-D NOSTATS.*\)$$$$/#\1/' \
	-e 's/\(`mm-config[^`]*\)--cflags\([^`]*`\)/\1\2/g' \
	-e 's/\(`mm-config[^`]*\)--ldflags\([^`]*`\)/\1\2/g' \
	-e 's/`mm-config *\(--libs\|--all\) *`/-lmm/g' \
	-e 's/`mm-config *`//g' \
	$(PKG_BUILD_DIR)/src/Makefile
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC=$(TARGET_CC) \
		USER_CFLAGS='$(TARGET_CFLAGS) $(EXTRA_CFLAGS) \
			$(TARGET_CPPFLAGS) $(EXTRA_CPPFLAGS) \
			-I$(STAGING_DIR)/usr/lib/libiconv-full/include' \
		USER_LDFLAGS='$(TARGET_LDFLAGS) $(EXTRA_LDFLAGS) \
			-L$(STAGING_DIR)/usr/lib/libiconv-full/lib'
endef

define Package/smstools3/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/smsd $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/scripts/sendsms $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DATA) -m 0755 ./files/sms3 $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DATA) ./files/smsd.conf $(1)/etc/
endef

$(eval $(call BuildPackage,smstools3))
