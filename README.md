# WolkConnect

## About

Example connector, with library source included,  written in C++ for WolkAbout
Smart IoT Cloud.

This version publishes data via MQTT protocol to
[WolkSense.com](https://wolksense.com/) cloud instance.

## Compiling WolkConnect-Cpp-

### Resolving dependencies

Dependencies for WolkConnect-Cpp- are resolved by invoking

    make dependencies


Former downloads and compiles all dependencies needed by WolkConnect-Cpp-.

All dependency compilation, and installation, is done inside 'dependencies'
folder, meaning no changes are made to operating system.

### Compiling

WolkConnect-Cpp- is compiled by invoking

    make all

If dependencies are not resolved prior this call, they are automatically
resolved.
