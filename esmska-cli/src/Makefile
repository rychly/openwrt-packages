service: lib
	make -C src/cli dynamic
	make -C src/service

cli:
	make -C src/libesmska -f Makefile.static
	make -C src/cli static

lib:
	make -C src/libesmska -f Makefile.dynamic

install:
	mkdir /usr/share/esmska
	cp esmska-cli /usr/bin
ifeq ($(wildcard esmskad),esmskad)
	cp esmskad /usr/bin
	cp libesmska.so.1.0.0 /lib
	ln -s /lib/libesmska.so.1.0.0 /lib/libesmska.so.1
	ln -s /lib/libesmska.so.1 /lib/libesmska.so
endif

uninstall:
	rm -rf /usr/share/esmska
	rm -f /usr/bin/esmska-cli
ifeq ($(wildcard /usr/bin/esmskad),esmskad)
	rm -f /usr/bin/esmskad /lib/libesmska.so*
endif

clean:
	rm objects/*
	rm -f esmska-cli esmskad libesmska.so.1.0.0
