CXXFLAGS=-Wall -Wextra
CXXFLAGS+= `pkg-config --cflags SDLP_event SDLP_tools SDLP_position SDL_image`
CXXFLAGS+= `sdl-config --cflags`
LDFLAGS=`pkg-config --libs SDLP_event SDLP_tools SDLP_position SDL_image`
LDFLAGS+= `sdl-config --libs`
LDFLAGS+= -ltinyxml -lboost_filesystem -lboost_system
CC=g++


