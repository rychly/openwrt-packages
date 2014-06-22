#
# Copyright (C) 2013 Marek Rychly
#
# This is free software, licensed under the GNU General Public License v2.
#
# To enable this package, copy the content of this directory into <openwrt>/package/network/utils/dropbrute
# and run: make -j5 -l4 tools/install toolchain/install package/network/utils/dropbrute/compile
#  -j option sets the maximum number of parallel jobs that can be run via make (#CPUs+1 to help ensure saturation of processor utilization)
#  -l prevents any new parallel job starting unless the load is below the amount specified
# the resulting package will be in directory: <openwrt>/bin/<target>/packages
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dropbrute
PKG_VERSION:=2014-06-16
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/dropbrute
	SECTION:=net
	CATEGORY:=Network
	TITLE:=A lightweight ssh brute force banning script
	URL:=https://forum.openwrt.org/viewtopic.php?id=32685
	MAINTAINER:=Marek Rychly <marek.rychly@gmail.com>
endef

define Package/dropbrute/description
	A lightweight shell script, runs out of cron, written in busybox ash.
	It should work completely out of the box with no additional packages required.
	The list of banned hosts can optionally persistent across reboots,
	you can permanently whitelist (or blacklist) hosts or networks,
	and the default expiration time ("lease time") is easily set.
	Thanks to robzr and arokh for the original code at
	https://forum.openwrt.org/viewtopic.php?pid=236965#p236965
endef

define Build/Compile
	true
endef

define Package/dropbrute/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/dropBrute.sh $(1)/usr/sbin/dropbrute
endef

$(eval $(call BuildPackage,dropbrute))
