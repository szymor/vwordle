#include "sound.hpp"
#include <SDL/SDL_mixer.h>
#include <stdlib.h>

#define MUSIC_PATH		"music/"
#define SFX_PATH		"sfx/"

Mix_Music *music = nullptr;
Mix_Chunk *silent = nullptr;
Mix_Chunk *loud = nullptr;

void loadSounds()
{
	if (rand() % 256 < 128)
	{
		music = Mix_LoadMUS(MUSIC_PATH "soundscape3.mp3");
	}
	else
	{
		music = Mix_LoadMUS(MUSIC_PATH "soundscape1.mp3");
	}
	silent = Mix_LoadWAV(SFX_PATH "click1.wav");
	loud = Mix_LoadWAV(SFX_PATH "click2.ogg");
}

void unloadSounds()
{
	Mix_FreeMusic(music);
	music = nullptr;
	Mix_FreeChunk(silent);
	silent = nullptr;
	Mix_FreeChunk(loud);
	loud = nullptr;
}

void playSilentClick()
{
	Mix_PlayChannel(-1, silent, 0);
}

void playLoudClick()
{
	Mix_PlayChannel(-1, loud, 0);
}

void playMusic()
{
	if (!Mix_PlayingMusic())
		Mix_PlayMusic(music, -1);
}

void setVolume(int volume)
{
	Mix_VolumeMusic(volume);
}
