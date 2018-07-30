#!/bin/bash
export PKG_CONFIG_PATH=$PWD/liballofw/install/lib/pkgconfig

cd allofw.node

npm install nan

mkdir -p binaries

node-gyp --target=v6.14.3 configure
make
cp build/Release/allofw.node binaries/allofw.v6.node

node-gyp --target=v7.10.1 configure
make
cp build/Release/allofw.node binaries/allofw.v7.node

node-gyp --target=v8.11.3 configure
make
cp build/Release/allofw.node binaries/allofw.v8.node

node-gyp --target=v9.11.2 configure
make
cp build/Release/allofw.node binaries/allofw.v9.node

node-gyp --target=v10.7.0 configure
make
cp build/Release/allofw.node binaries/allofw.v10.node

cd ../standalone
python depcollect-osx.py
