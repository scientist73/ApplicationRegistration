#!/bin/bash
apt install libdbus-1-3 -y
apt install libglib2.0-dev -y
apt install qt6-base-dev -y
apt install dbus-x11 -y
apt install cmake -y
apt install clang -y
cmake -S . -B build
cd build
cmake --build .
cmake --install .
