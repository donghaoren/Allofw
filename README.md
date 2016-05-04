# Allofw - AlloFramework

A framework for rendering 3D content for multi-projector display systems.

[Documentation](http://donghaoren.github.io/Allofw)

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

Open source, exact license is to be determined.
