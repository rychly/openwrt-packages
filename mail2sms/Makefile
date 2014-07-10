#
# Copyright (C) 2014 Marek Rychly
#
# This is free software, licensed under the GNU General Public License v2.
#
# To enable this package, copy the content of this directory into <openwrt>/package/utils/mail2sms
# and run: make -j5 -l4 tools/install toolchain/install package/utils/mail2sms/compile
#  -j option sets the maximum number of parallel jobs that can be run via make (#CPUs+1 to help ensure saturation of processor utilization)
#  -l prevents any new parallel job starting unless the load is below the amount specified
# the resulting package will be in directory: <openwrt>/bin/<target>/packages
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mail2sms
PKG_VERSION:=1.3.5
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://daniel.haxx.se/projects/mail2sms/

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/mail2sms
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Converts a single (large) mail to a tiny text.
	URL:=http://daniel.haxx.se/projects/mail2sms/
	MAINTAINER:=Marek Rychly <marek.rychly@gmail.com>
endef

define Package/mail2sms/description
	Converts a single (large) mail to a tiny text with contents from the mail.
	Perfectly suitable to send as an SMS message to a GSM telephone.
	This program acts as a filter and can optionally run
	another program and pass the filtered data into it.
	This program features:
	regex search/replace, abort on regexes, conditional regex rules,
	conditional output, conditional whatever actually ;-),
	change output length, control output format,
	full MIME mail support (including multipart/alternative),
	conditional runs dependent on date/time,
	splitting up the output in multiple output parts and more!
	This program can not actually send any SMS by itself.
endef

define Build/Prepare
	$(call Build/Prepare/Default)
endef

define Package/mail2sms/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/usr/share/man/man1
	$(INSTALL_DIR) $(1)/usr/share/man/man4
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mail2sms $(1)/usr/bin/
	$(INSTALL_DATA) ./files/mail2sms.conf $(1)/etc/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/mail2sms.1 $(1)/usr/share/man/man1/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/mail2sms.4 $(1)/usr/share/man/man4/
endef

$(eval $(call BuildPackage,mail2sms))
