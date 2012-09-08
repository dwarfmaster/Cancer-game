CXXFLAGS=-Wall -Wextra
CXXFLAGS+= `pkg-config --cflags SDLP_event SDLP_tools SDLP_position SDL_image SDL_mixer`
CXXFLAGS+= `sdl-config --cflags`
LDFLAGS=`pkg-config --libs SDLP_event SDLP_tools SDLP_position SDL_image SDL_mixer`
LDFLAGS+= `sdl-config --libs`
LDFLAGS+= -ltinyxml -lboost_filesystem -lboost_system -lboost_program_options
CC=g++


