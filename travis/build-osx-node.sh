#!/bin/bash
export PKG_CONFIG_PATH=$PWD/liballofw/install/lib/pkgconfig

cd allofw.node

node-gyp configure
make -C build
