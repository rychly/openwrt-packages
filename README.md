# OpenWrt packages

## Add the third-party source to your opkg configuration (ar71xx builds)

The packages are built from [OpenWrt development trunk for ar71xx/generic](https://downloads.openwrt.org/snapshots/trunk/ar71xx/generic/),
see the [SDK version](http://rychly.gitlab.io/openwrt-packages/openwrt-sdk.txt).

~~~sh
uclient-fetch -O /tmp/rychly_openwrt_packages.pub http://rychly.gitlab.io/openwrt-packages/openwrt-packages.pub && opkg-key add /tmp/rychly_openwrt_packages.pub
echo "src/gz rychly_openwrt_packages_base http://rychly.gitlab.io/openwrt-packages/base" >> /etc/opkg/customfeeds.conf
echo "src/gz rychly_openwrt_packages_kernel http://rychly.gitlab.io/openwrt-packages/kernel" >> /etc/opkg/customfeeds.conf
echo "src/gz rychly_openwrt_packages_telephony http://rychly.gitlab.io/openwrt-packages/telephony" >> /etc/opkg/customfeeds.conf
echo "src/gz rychly_openwrt_packages_packages http://rychly.gitlab.io/openwrt-packages/packages" >> /etc/opkg/customfeeds.conf
echo "src/gz rychly_openwrt_packages_routing http://rychly.gitlab.io/openwrt-packages/routing" >> /etc/opkg/customfeeds.conf
echo "src/gz rychly_openwrt_packages_luci http://rychly.gitlab.io/openwrt-packages/luci" >> /etc/opkg/customfeeds.conf
echo "src/gz rychly_openwrt_packages_management http://rychly.gitlab.io/openwrt-packages/management" >> /etc/opkg/customfeeds.conf
echo "# src/gz rychly_openwrt_packages_targets http://rychly.gitlab.io/openwrt-packages/targets" >> /etc/opkg/customfeeds.conf
~~~
