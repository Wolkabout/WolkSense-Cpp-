#!/bin/bash

# Copyright 2016 WolkAbout Technology s.r.o.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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

## libcurl
if [ ! -f "./build/lib/libcurl.so" ]; then
    echo "Building libcurl"

    cd libcurl && ./buildconf && ./configure --prefix=$BUILD_DIR               \
                                             --with-ssl=$BUILD_DIR             \
    && cd ..

    cd libcurl && make && make install && cd ..
fi

## restclient-cpp
if [ ! -f "./build/lib/librestclient-cpp.so" ]; then
    echo "Building restclient-cpp"

    cd restclient-cpp && ./autogen.sh && ./configure --prefix=$BUILD_DIR       \
                                                     --with-ssl=$BUILD_DIR     \
    && cd ..

    cd restclient-cpp && make install && cd ..
fi

# Copying necessary libraries to lib folder
cp -f $LIB_DIR/*.so*                 ../out/lib
