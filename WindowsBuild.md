# Building Allofw for Windows

Required Softwares

- Microsoft Visual Studio 2015 Update 3
- CMake
- NodeJS
- Python
- Boost library


## Checkout the Source

    git clone https://github.com/donghaoren/Allofw
    
    cd Allofw
    git submodule init
    git submodule update


## Build liballofw

### Step 1: Build Skia under `liballofw\dependencies\skia`

Run the following in command line:

    cd liballofw\dependencies\skia
    checkout.sh
    build.sh

If successful, you'll get `skia.dll` under `liballofw\dependencies\skia\skia\out\Release`.

### Step 2: Build liballofw under `liballofw`

Run the following in command line:

    cd liballofw
    mkdir build
    cd build
    cmake .. -G "Visual Studio 14 2015 Win64" -DBOOST_ROOT=<path-to-your-boost-download>

Build liballofw:

    cmake --build . --config Release

You'll get several dlls under `lib\Release`:

- allofw.dll
- allofw-graphics.dll
- glfw3.dll
- lodepng.dll
- yaml-cpp.dll


## Build allofw.node

Run the following in command line:

    cd allofw.node

Build the bindings:

    npm install

You'll get `allofw.node` under `build\Release`.

To use the generated binaries, you need to copy the dlls previously generated (skia and liballofw), as well as the dlls under `liballofw\dependencies\windows-binaries` to the folder where `allofw.node` lives.


## Build allofw-openvr.node

Run the following in command line:

    cd allofw-openvr.node

    # Download OpenVR source
    git clone https://github.com/ValveSoftware/openvr.git

Build the bindings:

    npm install

You'll get `allofw_openvr.node` under `build\Release`.

To use the generated binaries, you need to copy the dlls previously generated (skia and liballofw), as well as the dlls under `liballofw\dependencies\windows-binaries` to the folder where `allofw-openvr.node` lives.


## Putting Everything Together

Run `WindowsPackage.bat` in the main directory, you'll get everything under `win32_x64`.
