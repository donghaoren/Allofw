#!/bin/bash

cd liballofw
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install -DWITH_SKIA=NO
make -j8
make install

