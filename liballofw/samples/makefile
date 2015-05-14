# A sample makefile that uses liballofw.

CXXFLAGS = -std=c++11 -I../include -L../lib

# Detect OS.
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LDFLAGS += -lGL -lallofw
    INSTALL_NAME_TOOL = true
    CXX = g++
endif
ifeq ($(UNAME_S),Darwin)
    LDFLAGS += -framework OpenGL -lallofw -lallofw-omniapp
    INSTALL_NAME_TOOL = install_name_tool
    CXX = clang++
endif

samples: cubes cubes_bcast

cubes: cubes.cpp
	g++ cubes.cpp $(CXXFLAGS) $(LDFLAGS) -o cubes
	$(INSTALL_NAME_TOOL) -change liballofw.dylib ../lib/liballofw.dylib cubes
	$(INSTALL_NAME_TOOL) -change liballofw-omniapp.dylib ../lib/liballofw-omniapp.dylib cubes

cubes_bcast: cubes_bcast.cpp
	g++ cubes_bcast.cpp $(CXXFLAGS) $(LDFLAGS) -o cubes_bcast
	$(INSTALL_NAME_TOOL) -change liballofw.dylib ../lib/liballofw.dylib cubes_bcast
	$(INSTALL_NAME_TOOL) -change liballofw-omniapp.dylib ../lib/liballofw-omniapp.dylib cubes_bcast
