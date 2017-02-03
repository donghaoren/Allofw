#!/bin/bash

export PATH="${PWD}/depot_tools:${PATH}"
cd skia
git checkout chrome/m57
python tools/git-sync-deps
bin/gn gen out/Release  --args='cc="clang" cxx="clang++" is_official_build=true is_component_build=true is_debug=false extra_cflags_cc=["-frtti"]'
ninja -C out/Release