#!/bin/sh

g++ -g ../src/main.cpp -I../include -I../src -L../lib -lfreenect -L/usr/lib64 -I/usr/include/libusb-1.0 -Wl,-rpath ../lib &&
echo Done build
