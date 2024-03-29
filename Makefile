.PHONY: all clean

PROJECT=vwordle
SRCPATH=src
INCPATH=inc
SRC=main.cpp states.cpp sound.cpp definitions.cpp
SRC:=$(addprefix $(SRCPATH)/, $(SRC))
INC=global.hpp states.hpp sound.hpp definitions.hpp
INC:=$(addprefix $(INCPATH)/, $(INC))
CXX=g++
CFLAGS=$(shell pkg-config --cflags sdl SDL_image SDL_mixer SDL_ttf sqlite3) -I$(INCPATH)
LFLAGS=$(shell pkg-config --libs sdl SDL_image SDL_mixer SDL_ttf sqlite3)

all: $(PROJECT)

$(PROJECT): $(SRC) $(INC)
	$(CXX) -o $(PROJECT) $(CFLAGS) $(SRC) $(LFLAGS)

clean:
	rm -rf $(PROJECT)
