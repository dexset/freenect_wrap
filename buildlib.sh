#!/bin/bash
echo Building lib...
g++ -w -c -I./include -I./src -L./lib src/device.cpp -I/usr/include/libusb-1.0 -o lib/device.o &&
g++ -w -c -I./include -I./src -L./lib src/cwrap.cpp -I/usr/include/libusb-1.0 -o lib/cwrap.o &&
ar rcs lib/libfreenect_wrap.a lib/device.o lib/cwrap.o &&
rm lib/cwrap.o lib/device.o &&
export LD_LIBRARY_PATH="$(pwd)/lib"
echo Done
