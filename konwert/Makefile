#
# Copyright (C) 2014 Marek Rychly
#
# This is free software, licensed under the GNU General Public License v2.
#
# To enable this package, copy the content of this directory into <openwrt>/package/utils/konwert
# and run: make -j5 -l4 tools/install toolchain/install package/utils/konwert/compile
#  -j option sets the maximum number of parallel jobs that can be run via make (#CPUs+1 to help ensure saturation of processor utilization)
#  -l prevents any new parallel job starting unless the load is below the amount specified
# the resulting package will be in directory: <openwrt>/bin/<target>/packages
#

include $(TOPDIR)/rules.mk

PKG_NAME:=konwert
PKG_VERSION:=1.8.11.2
PKG_RELEASE:=1

PV_MAJ:=1.8
PV_MIN:=11.2

PKG_SOURCE:=$(PKG_NAME)_$(PV_MAJ).orig.tar.gz
PKG_SOURCE_URL:=http://ftp.debian.org/debian/pool/main/k/konwert/
# and patches in
# $(PKG_SOURCE_URL)$(PKG_NAME)_$(PV_MAJ)-$(PV_MIN).diff.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PV_MAJ)

include $(INCLUDE_DIR)/package.mk

define Package/konwert
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Charset conversion for files or terminal I/O.
	URL:=http://packages.qa.debian.org/konwert
	MAINTAINER:=Marek Rychly <marek.rychly@gmail.com>
endef

define Package/konwert/description
	Konwert provides charset conversion for files or terminal I/O.
	Filters used by konwert for charset conversion are included.
endef

define Build/Prepare
	$(call Build/Prepare/Default)
endef

define Package/konwert/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/lib/$(PKG_NAME)/aux
	$(INSTALL_DIR) $(1)/usr/share/doc/$(PKG_NAME)-$(PV_MAJ)
	$(INSTALL_DIR) $(1)/usr/share/man/man1
	$(INSTALL_DIR) $(1)/usr/share/$(PKG_NAME)
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/install/bin/* $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/install/lib/aux/* $(1)/usr/lib/$(PKG_NAME)/aux/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/install/doc/en/* $(1)/usr/share/doc/$(PKG_NAME)-$(PV_MAJ)/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/install/man/man1/* $(1)/usr/share/man/man1/
	cp -r $(PKG_BUILD_DIR)/install/share/* $(1)/usr/share/$(PKG_NAME)/
endef

$(eval $(call BuildPackage,konwert))
