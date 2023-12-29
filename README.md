# wizlightcpp

## Project Overview
A native application to manage and control the [Wiz](https://www.wizconnected.com/en-us) light devices. This project is developed using native C/C++ language so that it can be ported to any environment.

## About Wiz
WiZ is an IoT platform for smart lighting solutions and smart services. WiZ delivers a Wi-Fi-based smart system that controls plug & play devices with our user-friendly and feature-rich app, and that connects to existing Wi-Fi networks. It also provides ways to dynamic change the light color. [Source](https://www.wizconnected.com/en-us/about-wiz)

## License
wizlightcpp is licensed under the MIT license; see LICENSE in the source distribution for details.


## Prerequisites
Before building, you'll need to install following dependencies

    cmake
    libjansson-dev
    nlohmann
    qt5
    arp

## Install nlohman
    sudo apt-get install nlohmann-json3-dev
    sudo apt-get install qtbase5-dev
    sudo apt-get install net-tools

## Building
wizlightcpp should work with any Linux distribution and added cross compilation support.

    Tested on:
        X86_64
        Arm
        macOS

### Checking out the wizlightcpp
To check out the wizlightcpp repository:

    git clone https://github.com/srisham/wizlightcpp.git

### Build
Use cmake tools to build the wizlightcpp. Run the following commands to build the app:

    mkdir build
    cd build
    cmake ..
    make

### Cross Compilation
In order to cross compile this module, use the cmake-tookchains file available in the [toolchain](cmake/toolchain) directory. Update the arm_toolchain.cmake file with your toolchain details. Refer the sample version in this [example](cmake/toolchain/example).
To cross-compile, run the following commands:

    mkdir build_arm
    cd build_arm
    cmake  -DCMAKE_TOOLCHAIN_FILE=<path_to_toolchain_cmake_file>/arm_toolchain.cmake ..
    make

## Usage
wizlightcpp supports both interactive and cli methods.

If only 'command' passed as input parameter, it will enable the app in interactive mode. User can enter the command specific inputs to customize the bulb.

    $ wizlightcpp on

In cli mode, user can all pass necessary 'options' as arguments in one go.

    $ wizlightcpp setbrightness --ip 192.168.20.105 --dim 50

To know the list of 'options' required for each command, run the 'help' option in the following way:

    $ wizlightcpp off --help

## Supported Features
Following features are supported now:

    Auto discovery of Wiz light
    Toggle light
    Get Status
    Get Device Info
    Reboot
    Get User Configuration
    Get Wifi Configuration
    Get Model Configuration
    Get System Configuration
    Set Brightness
    Set RGB Color
    Set Color Temperature
    Set Speed
    Set SceneId

