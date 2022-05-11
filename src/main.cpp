#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include "global.hpp"
#include "states.hpp"
#include "sound.hpp"

SDL_Surface *screen = nullptr;

int main(int argc, char *argv[])
{
	srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_EnableKeyRepeat(300, 100);
	SDL_ShowCursor(SDL_DISABLE);

	Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 1024);
	loadSounds();

	while (SI_QUIT != stateid)
	{
		switch (stateid)
		{
			case SI_MENU:
				setVolume(VOL_HALF);
				menustate.loadGfx();
				menustate.processAll();
				menustate.unloadGfx();
				break;
			case SI_GAME:
				playMusic();
				setVolume(VOL_FULL);
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
			case SI_QUIT:
				// no action
				break;
		}
	}

	unloadSounds();
	SDL_Quit();
	return 0;
}
