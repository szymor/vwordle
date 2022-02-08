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
	SDL_EnableKeyRepeat(300, 100);
	SDL_ShowCursor(SDL_DISABLE);
	menustate.loadGfx();
	gamestate.loadGfx();
	gamestate.loadDictionary();

	while (SI_QUIT != stateid)
	{
		switch (stateid)
		{
			case SI_MENU:
				menustate.processAll();
				break;
			case SI_GAME:
				gamestate.resetGame();
				gamestate.processAll();
				break;
		}
	}

	menustate.unloadGfx();
	gamestate.unloadGfx();
	SDL_Quit();
	return 0;
}
