#!/bin/bash

#  Defining base project directory
BASE_DIR="$(readlink -f ../)"
INCLUDE_DIR="$BASE_DIR/include"

#  Defining build directory
BUILD_DIR="$BASE_DIR/dependencies/build"
LIB_DIR="$BUILD_DIR/lib"

export PATH=$BUILD_DIR/bin:$PATH
mkdir -p $BUILD_DIR

## m4
if [ ! -f "./build/bin/m4" ]; then
    echo "Building m4"

    cd m4 && ./configure --prefix=$BUILD_DIR && cd ..
    cd m4 && make && make install && cd ..
fi

## help2man
if [ ! -f "./build/bin/help2man" ]; then
    echo "Building help2man"

    cd help2man && ./configure --prefix=$BUILD_DIR && cd ..
    cd help2man && make && make install && cd ..
fi

## autoconf
if [ ! -f "./build/bin/autoconf" ]; then
    echo "Building autoconf"

    cd autoconf && ./configure --prefix=$BUILD_DIR && cd ..
    cd autoconf && make && make install && cd ..
fi

## automake
if [ ! -f "./build/bin/automake" ]; then
    echo "Building modbus"

    cd automake && ./configure --prefix=$BUILD_DIR && cd ..
    cd automake && make && make install && cd ..
fi

## libtool
if [ ! -f "./build/bin/libtool" ]; then
    echo "Building libtool"

    cd libtool && ./bootstrap && ./configure --prefix=$BUILD_DIR && cd ..
    cd libtool && make && make install && cd ..
fi

## libssl
if [ ! -f "./build/lib/libssl.so" ]; then
    echo "Building libssl"

    cd libssl && ./config enable-egd enable-heartbeats enable-ssl3             \
           enable-ssl3-method enable-md2 enable-rc5 enable-rfc3779             \
           enable-shared enable-ssl2 enable-weak-ssl-ciphers enable-zlib       \
           enable-zlib-dynamic                                                 \
           --prefix=$BUILD_DIR && cd ..

    cd libssl && make && make install && cd ..
fi

## paho.mqtt.c
if [ ! -f "./build/lib/libpaho-mqtt3as.so" ]; then
    echo "Building paho.mqtt.c"

    export LDFLAGS="-L$LIB_DIR"
    export CFLAGS="-I$BUILD_DIR/include"
    export LD_LIBRARY_PATH=$LIB_DIR
    export LIBRARY_PATH=$LIB_DIR

    cd paho.mqtt.c && make && cd ..
    cp -d paho.mqtt.c/build/output/*.so* $LIB_DIR

    rm -rf  $INCLUDE_DIR/mqtt
    mkdir $INCLUDE_DIR/mqtt

    unset LDFLAGS
    unset CFLAGS
    unset LD_LIBRARY_PATH
    unset LIBRARY_PATH
fi

## paho.mqtt.cpp
if [ ! -f "./build/lib/libmqttpp.so" ]; then
    echo "Building libmqttpp"

    cd paho.mqtt.cpp && make PAHO_C_HEADERS=$BUILD_DIR/include && cd ..

    sed -i                                                                     \
    "s/\bMQTTAsync_connectOptions\b/public:MQTTAsync_connectOptions/g"         \
    paho.mqtt.cpp/src/mqtt/connect_options.h

    cp -d paho.mqtt.cpp/lib/* $LIB_DIR
    cp -d paho.mqtt.cpp/src/mqtt/* $INCLUDE_DIR/mqtt
fi

## restclient-cpp
if [ ! -f "./build/lib/librestclient-cpp.so" ]; then
    echo "Building restclient-cpp"

    cd restclient-cpp && ./autogen.sh && ./configure --prefix=$BUILD_DIR &&    \
    cd ..

    cd restclient-cpp && make install && cd ..
fi

# Copying necessary libraries to lib folder
cp -f $LIB_DIR/*.so*                 ../out/lib