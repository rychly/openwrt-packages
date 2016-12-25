# OpenWrt packages

## Add the third-party source to your opkg configuration (ar71xx builds)

~~~sh
uclient-fetch -O /tmp/rychly-openwrt-packages.pub http://rychly.gitlab.io/openwrt-packages/openwrt-packages.pub && opkg-key add /tmp/rychly-openwrt-packages.pub
echo "src/gz rychly-openwrt-packages http://rychly.gitlab.io/openwrt-packages" >> /etc/opkg/customfeeds.conf
~~~
