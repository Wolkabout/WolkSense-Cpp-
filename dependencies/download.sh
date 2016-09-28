#!/bin/bash

## m4
if [ ! -d "m4" ]; then
	echo "Downloading m4"
	wget -O m4.tar.gz http://ftp.gnu.org/gnu/m4/m4-1.4.17.tar.gz
	tar -xvzf m4.tar.gz
	mv m4-1.4.17 m4
	rm m4.tar.gz
fi

## help2man
if [ ! -d "help2man" ]; then
	echo "Downloading help2man"
	wget -O help2man.tar.gz http://mirror.sbb.rs/gnu/help2man/help2man-1.43.3.tar.gz
	tar -xvzf help2man.tar.gz
	mv help2man-1.43.3 help2man
	rm help2man.tar.gz
fi

## autoconf
if [ ! -d "autoconf" ]; then
	echo "Downloading autoconf"
	wget -O autoconf.tar.gz http://ftp.gnu.org/gnu/autoconf/autoconf-2.69.tar.gz
	tar -xvzf autoconf.tar.gz
	mv autoconf-2.69 autoconf
	rm autoconf.tar.gz
fi

## automake
if [ ! -d "automake" ]; then
	echo "Downloading automake"
	wget -O automake.tar.gz http://ftpmirror.gnu.org/automake/automake-1.15.tar.gz
	tar -xvzf automake.tar.gz
	mv automake-1.15 automake
	rm automake.tar.gz
fi

## libtool
if [ ! -d "libtool" ]; then
	echo "Downloading libtool"
	wget -O libtool.tar.gz http://mirror.sbb.rs/gnu/libtool/libtool-2.4.tar.gz
	tar -xvzf libtool.tar.gz
	mv libtool-2.4 libtool
	rm libtool.tar.gz
fi

## libssl
if [ ! -d "libssl" ]; then
	echo "Downloading libssl"
	wget -O openssl.tar.gz https://github.com/Wolkabout/openssl/archive/OpenSSL_1_0_2i.tar.gz
	tar -xvzf openssl.tar.gz
	mv openssl-OpenSSL_1_0_2i libssl
	rm openssl.tar.gz
fi

# paho.mqtt.c
if [ ! -d "paho.mqtt.c" ]; then
	echo "Downloading paho.mqtt.c"
	wget -O paho.mqtt.c.tar.gz https://github.com/eclipse/paho.mqtt.c/archive/v1.1.0.tar.gz
	tar -xvzf paho.mqtt.c.tar.gz
	mv paho.mqtt.c-1.1.0 paho.mqtt.c
	rm paho.mqtt.c.tar.gz
fi

# paho.mqtt.cpp
if [ ! -d "paho.mqtt.cpp" ]; then
	echo "Downloading paho.mqtt.cpp"
	wget -O org.eclipse.paho.mqtt.cpp.tar.gz https://github.com/Wolkabout/paho.mqtt.cpp/archive/1.0.tar.gz
	tar -xvzf org.eclipse.paho.mqtt.cpp.tar.gz
	mv paho.mqtt.cpp-1.0 paho.mqtt.cpp
	rm org.eclipse.paho.mqtt.cpp.tar.gz
fi

# libcurl
if [ ! -d "libcurl" ]; then
	echo "Downloading libcurl"
	wget -O libcurl.tar.gz https://github.com/curl/curl/archive/curl-7_50_3.tar.gz
	tar -xvzf libcurl.tar.gz
	mv curl-curl-7_50_3 libcurl
	rm libcurl.tar.gz
fi

# restclient-cpp
if [ ! -d "restclient-cpp" ]; then
	echo "Downloading paho.mqtt.cpp"
	wget -O restclient-cpp.tar.gz https://github.com/mrtazz/restclient-cpp/archive/0.4.4.tar.gz
	tar -xvzf restclient-cpp.tar.gz
	mv restclient-cpp-0.4.4 restclient-cpp
	rm restclient-cpp.tar.gz
fi
