#!/bin/bash

export PATH="${PWD}/depot_tools:${PATH}"
cd skia
git checkout chrome/m38_2125
./tools/git-sync-deps
make BUILDTYPE=Release
