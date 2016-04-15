#!/bin/bash

export PATH="${PWD}/depot_tools:${PATH}"
cd skia
git checkout chrome/m50
./bin/sync-and-gyp
ninja -C out/Release
