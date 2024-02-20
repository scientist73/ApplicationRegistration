#!/bin/bash
apt install libdbus-1-3
apt install libglib2.0-dev
apt install qt6-base-dev
apt install dbus-x11
apt install cmake
apt install clang
cmake -S . -B build
cd build
cmake --build .
cmake --install .
