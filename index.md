---
title: Allofw Documentation
layout: default
---

Allofw
====

Allofw is a framework for rendering 3D content for multi-projector display systems.

Components
----

Allofw consists of the following components.

- liballofw: C++ Library for OpenGL window creation, OmniStereo rendering and 2D Graphics.

	* [Documentation](docs/liballofw/html/annotated.html)

- allofw.node: Node.js bindings for liballofw.

	* [Documentation](docs/allofw.node/index.html)

- allosphere.node: AlloSphere specific components for Node.js.

	* [Documentation](docs/allosphere.node/index.html)


Dependencies
----

- Node.js: 4.0+
- yaml-cpp
- GLFW3
- lodepng
- Boost (headers only)
- Skia

## Build

The Allofw library contains liballofw and its Node.js bindings allofw.node. You should build then in the following order.

### Build liballofw

#### 1. Initialize and checkout dependencies.

    git submodule init
    git submodule update

#### 2. (optional) If you need Skia graphics support, do the following:

    cd liballofw/dependencies/skia
    ./checkout.sh

    # If you are using linux, look into instructions_for_linux.txt,
    # there are two changes in the source that needs to be made.

    ./build.sh

#### 3. Install necessary packages:

For Mac with homebrew:

    brew install boost
    brew install ffmpeg

For ubuntu:

    sudo apt-get install ffmpeg
    sudo apt-get install libboost-all-dev

#### 4. Build liballofw

    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install
    # or "cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install -DWITH_SKIA=NO" if you didn't build Skia.
    make -j8
    make install

This will install liballofw's shared libraries and header files to the directory you specified.

### Build allofw.node

You'll need Node.js 4.0+ for this to compile.

    # Setup where allofw was installed.
    export PKG_CONFIG_PATH=/path/to/allofw/install/lib/pkgconfig:$PKG_CONFIG_PATH
    cd allofw.node
    node-gyp configure
    make -C build

After you have allofw.node built, you can do the following:

For Mac:

    export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/path/to/allofw/install/lib
    export DYLD_FALLBACK_LIBRARY_PATH=$DYLD_LIBRARY_PATH
    export NODE_PATH=$NODE_PATH:/path/to/allofw.node/build/Release

For Linux, just replace `DYLD_LIBRARY_PATH` by `LD_LIBRARY_PATH`.

Then you should be able to run the samples in `allofw.node/samples` with the node command.

## License

Copyright (c) 2015-2016 Donghao Ren

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
