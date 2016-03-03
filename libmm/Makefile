#
# Copyright (C) 2016 Marek Rychly
#
# This is free software, licensed under the GNU General Public License v2.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libmm
PKG_VERSION:=1.4.2
PKG_RELEASE:=1

PKG_SOURCE:=mm-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=ftp://ftp.ossp.org/pkg/lib/mm

PKG_BUILD_DIR:=$(BUILD_DIR)/mm-$(PKG_VERSION)

PKG_FIXUP:=autoreconf

include $(INCLUDE_DIR)/package.mk

define Package/libmm
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE:=OSSP mm Shared Memory Allocation Library
	URL:=http://www.ossp.org/pkg/lib/mm/
	MAINTAINER:=Marek Rychly <marek.rychly@gmail.com>
endef

define Package/libmm/description
	OSSP mm is a 2-layer abstraction library which simplifies the usage of
	shared memory between forked (and this way strongly related) processes
	under Unix platforms. On the first layer it hides all platform dependent
	implementation details (allocation and locking) when dealing with shared
	memory segments and on the second layer it provides a high-level
	malloc(3)-style API for a convenient and well known way to work with data
	structures inside those shared memory segments.
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	# need to upgrade *.ac to 2.50, see https://mail-archives.apache.org/mod_mbox/apr-dev/200107.mbox/%3CPine.SOL.3.91.1010708145414.13586A-200000@cse.cygnus.com%3E
	sed -i 's/AC_CONFIGURE_PART(/AC_MSG_RESULT(/g' $(PKG_BUILD_DIR)/configure.ac
endef

define Package/libmm/install
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/mm.h $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libmm.la $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libmm))
