#!/bin/bash

export PATH="${PWD}/depot_tools:${PATH}"
cp SkUserConfig.h skia/include/config
cd skia
python tools/git-sync-deps
bin/gn gen out/Release  --args='cc="clang" cxx="clang++" is_official_build=true is_component_build=true skia_use_system_expat=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_system_icu=false is_debug=false extra_cflags_cc=["-frtti"]'
ninja -C out/Release
