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

	while (SI_QUIT != stateid)
	{
		switch (stateid)
		{
			case SI_MENU:
				menustate.loadGfx();
				menustate.processAll();
				menustate.unloadGfx();
				break;
			case SI_GAME:
				gamestate.loadGfx();
				gamestate.loadDictionary(menustate.getWordLength());
				gamestate.resetGame();
				gamestate.processAll();
				gamestate.unloadGfx();
				break;
			case SI_RULES:
				rulesstate.loadGfx();
				rulesstate.processAll();
				rulesstate.unloadGfx();
				break;
		}
	}

	SDL_Quit();
	return 0;
}
