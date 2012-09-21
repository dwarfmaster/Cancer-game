RCDIR=/usr/share/cancer-game/
CXXFLAGS=-Wall -Wextra
CXXFLAGS+= `pkg-config --cflags SDLP_event SDLP_tools SDLP_position SDL_image SDL_mixer guichan-0.8 guichan_sdl-0.8`
CXXFLAGS+= `sdl-config --cflags`
CXXFLAGS+= -DRCDIR="\"$(RCDIR)\""
LDFLAGS=`pkg-config --libs SDLP_event SDLP_tools SDLP_position SDL_image SDL_mixer guichan-0.8 guichan_sdl-0.8`
LDFLAGS+= `sdl-config --libs`
LDFLAGS+= -ltinyxml -lboost_filesystem -lboost_system -lboost_program_options -lboost_regex
CC=g++


