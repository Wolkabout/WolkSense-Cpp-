# WolkConnect

## About

Example connector, with library source included, written in C++ for WolkSense 
Sensor Data Cloud.

Provided source is intended to be compiled and ran on debian-like systems.

This version publishes data via MQTT protocol to
[WolkSense Sensor Data Cloud](https://wolksense.com/) cloud instance.


## Compiling WolkConnect-Cpp-

### Resolving dependencies

Prior to resolving WolkConnect-Cpp- dependencies, following system packages must
be installed:

* wget
* build-essential
* zlib1g-dev

Installing these packages can be done by invoking:

    apt-get install wget build-essential zlib1g-dev

Afterwards WolkConnect-Cpp- dependencies are resolved by invoking:

    make dependencies

Former downloads and compiles all dependencies needed by WolkConnect-Cpp-.

All dependency compilation, and installation, is done inside 'dependencies'
folder, meaning no changes are made to operating system.

### Compiling

WolkConnect-Cpp- is compiled by invoking:

    make all

If dependencies are not resolved prior this call, they will be resolved
automatically

Compiled executable is located inside 'out/bin' directory


## Example

Provided example can activate device on WolkSense Sensor Data Cloud, and publish
sample readings, depending on invocation.

### Activating new device

To activate device invoke compiled 'wolksense-cpp-example', in 'out/bin',
directory in following manner

    ./wolksense-cpp-example {wolksenseEmail} {wolksensePassword} {deviceName}

For example:

    ./wolksense-cpp-example "mail@wolksense.com" "acc_pass" "New example Device"

This will create new device on WolkSense Sensor Data Cloud, show us assigned
device serial and password, which can be used for further publishing, and
publish sample readings which can be seen on
[WolkSense.com](https://wolksense.com/) under account that created device.

### Using activated device

If device is already activated, and device serial, and password, are known,
'wolksense-cpp-example' can be invoked to publish sample readings in following
manner:

    ./wolksense-cpp-example {device_serial} {device_password}

Prior will publish sample readings for already present device on WolkSense Sensor 
Data Cloud.
