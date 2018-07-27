#!/bin/bash

set PATH=%cd%\depot_tools;%PATH%
cp SkUserConfig.h skia\include\config
cd skia
python tools/git-sync-deps
bin\gn gen out/Release --args="is_official_build=true is_component_build=true is_debug=false extra_cflags_cc=[\"-w\"] extra_ldflags=[\"gdi32.lib\",\"usp10.lib\"]"
ninja -C out/Release