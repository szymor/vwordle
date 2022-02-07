.PHONY: all clean

PROJECT=vwordle
SRC=main.cpp states.cpp
CC=g++
CFLAGS=$(shell pkg-config --cflags sdl SDL_image)
LFLAGS=$(shell pkg-config --libs sdl SDL_image)

all: $(PROJECT)

$(PROJECT): $(SRC)
	$(CC) -o $(PROJECT) $(CFLAGS) $(SRC) $(LFLAGS)

clean:
	rm -rf $(PROJECT)
