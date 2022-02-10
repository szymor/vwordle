.PHONY: all clean

PROJECT=vwordle
SRCPATH=src
INCPATH=inc
SRC=main.cpp states.cpp
SRC:=$(addprefix $(SRCPATH)/, $(SRC))
INC=global.hpp states.hpp
INC:=$(addprefix $(INCPATH)/, $(INC))
CXX=g++
CFLAGS=$(shell pkg-config --cflags sdl SDL_image) -I$(INCPATH)
LFLAGS=$(shell pkg-config --libs sdl SDL_image)

all: $(PROJECT)

$(PROJECT): $(SRC) $(INC)
	$(CXX) -o $(PROJECT) $(CFLAGS) $(SRC) $(LFLAGS)

clean:
	rm -rf $(PROJECT)
