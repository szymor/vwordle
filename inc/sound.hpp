#ifndef _HPP_SOUND
#define _HPP_SOUND

#define VOL_FULL	(MIX_MAX_VOLUME / 3)
#define VOL_HALF	(MIX_MAX_VOLUME / 6)
#define VOL_MUTE	(0)

void loadSounds();
void unloadSounds();
void playSilentClick();
void playLoudClick();
void playMusic();
void setVolume(int volume);

#endif
