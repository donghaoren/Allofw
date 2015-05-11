CXXFLAGS = -std=c++11 -I../include -L../lib
LDFLAGS  = -lallofw

CXXFLAGS += -framework OpenGL

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CXXFLAGS += -lGL
endif
ifeq ($(UNAME_S),Darwin)
    CXXFLAGS += -framework OpenGL
endif

cubes: cubes.cpp
	g++ cubes.cpp $(CXXFLAGS) $(LDFLAGS) -o cubes
