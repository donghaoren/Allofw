CXXFLAGS = -std=c++11 -I../include -L../lib

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LDFLAGS += -lGL -lallofw
endif
ifeq ($(UNAME_S),Darwin)
    LDFLAGS += -framework OpenGL -lallofw
endif

cubes: cubes.cpp
	g++ cubes.cpp $(CXXFLAGS) $(LDFLAGS) -o cubes
	ifeq ($(UNAME_S),Darwin)
	    install_name_tool -change liballofw.dylib ../lib/liballofw.dylib cubes
	endif
