#
# Copyright (C) 2016 Marek Rychly
#
# This is free software, licensed under the GNU General Public License v2.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mm
PKG_VERSION:=1.4.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
# ftp.ossp.org returns often ERR_CONNECTION_REFUSED, use alternative site
#PKG_SOURCE_URL:=ftp://ftp.ossp.org/pkg/lib/$(PKG_NAME)
PKG_SOURCE_URL:=http://pkgs.fedoraproject.org/repo/pkgs/$(PKG_NAME)/$(PKG_SOURCE)/bdb34c6c14071364c8f69062d2e8c82b

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/mm
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE:=OSSP mm Shared Memory Allocation Library
	URL:=http://www.ossp.org/pkg/lib/mm/
	MAINTAINER:=Marek Rychly <marek.rychly@gmail.com>
endef

define Package/mm/description
	OSSP mm is a 2-layer abstraction library which simplifies the usage of
	shared memory between forked (and this way strongly related) processes
	under Unix platforms. On the first layer it hides all platform dependent
	implementation details (allocation and locking) when dealing with shared
	memory segments and on the second layer it provides a high-level
	malloc(3)-style API for a convenient and well known way to work with data
	structures inside those shared memory segments.
endef

ifndef MM_SHM_MAXSEGSIZE

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/maxsegsize -DTEST_SHMGET ./files/maxsegsize.c
	$(error Copy $(PKG_BUILD_DIR)/maxsegsize into the OpenWrt device and execute it to determine maximum shared memory segment size (usually 67108864) which should be set as MM_SHM_MAXSEGSIZE before making this package (e.g., make MM_SHM_MAXSEGSIZE=67108864 ...).)
endef

else

CONFIGURE_ARGS += --with-shm=IPCSHM
CONFIGURE_VARS += ac_cv_maxsegsize=$(MM_SHM_MAXSEGSIZE)

define Build/Prepare
	$(call Build/Prepare/Default)
# FIXME: aclocal.m4 makes the autoconf files compatible with autoconf version>=2.5, however, it is removed by OpenWRT toolchain if PKG_FIXUP:=autoreconf is set
# and then the autoreconf fails on missing definitions, so we will backup the aclocal.m4 file and include it into configure.in -- but it still fails to configure
#	$(CP) $(PKG_BUILD_DIR)/aclocal.m4 $(PKG_BUILD_DIR)/aclocal-dist.m4
endef

endif

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) DESTDIR="$(PKG_INSTALL_DIR)" install
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/mm.h $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libmm.{a,so*} $(1)/usr/lib/
endef

define Package/mm/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/*.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,mm))
