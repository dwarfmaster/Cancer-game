CXXFLAGS=-Wall -Wextra
CXXFLAGS+= `pkg-config --cflags SDLP_event SDLP_tools SDLP_position`
CXXFLAGS+= `sdl-config --cflags`
LDFLAGS=`pkg-config --libs SDLP_event SDLP_tools SDLP_position`
LDFLAGS+= `sdl-config --libs`
CC=g++


