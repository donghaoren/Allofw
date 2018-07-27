#!/bin/bash

cd liballofw

cd dependencies/skia
./checkout.sh
./build.sh

cd ../../

mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install -DWITH_SKIA=YES
make -j8
make install

