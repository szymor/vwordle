#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "global.hpp"
#include "states.hpp"

SDL_Surface *screen = nullptr;

int main(int argc, char *argv[])
{
	srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_ShowCursor(SDL_DISABLE);
	gamestate.loadGfx();
	gamestate.loadDictionary();

	gamestate.resetGame();
	while (SI_GAME == stateid)
	{
		gamestate.draw();
		gamestate.processInput();
	}

	gamestate.unloadGfx();
	SDL_Quit();
	return 0;
}
