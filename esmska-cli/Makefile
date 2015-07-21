#
# Copyright (C) 2015 Marek Rychly
#
# This is free software, licensed under the GNU General Public License v2.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=esmska-cli
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

PV_MAJ:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_CHECK_FORMAT_SECURITY:=0

include $(INCLUDE_DIR)/package.mk

define Package/esmska-cli
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=An application sending SMS messages via web gateways (CLI version).
	URL:=
	DEPENDS:=+libcurl
	MAINTAINER:=Marek Rychly <marek.rychly@gmail.com>
endef

define Package/esmska-cli/description
	An application sending SMS messages via web gateways (CLI version).
	It includes esmska-cli console application, esmskad daemon and libesmska.so library.
	It requires *.gateway files in /usr/share/esmska which can be downloaded from
	https://github.com/kparal/esmska/tree/master/gateways
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR) $(PKG_BUILD_DIR)/objects
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC='$(TARGET_CC) $(TARGET_CFLAGS)' \
		service OPTIONS='-D_REQUEST_CHECK_SOURCE -D_CHECK_JSON'
endef

define Package/esmska-cli/install
	mkdir -p $(1)/usr/bin $(1)/usr/lib $(1)/usr/share/esmska
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/esmska-cli $(PKG_BUILD_DIR)/esmskad $(1)/usr/bin/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/libesmska.so.$(PKG_VERSION) $(1)/usr/lib/
	ln -s libesmska.so.$(PKG_VERSION) $(1)/usr/lib/libesmska.so.$(PV_MAJ)
	ln -s libesmska.so.$(PV_MAJ) $(1)/usr/lib/libesmska.so
endef

$(eval $(call BuildPackage,esmska-cli))
