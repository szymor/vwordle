#ifndef _HPP_GLOBAL
#define _HPP_GLOBAL

#include <SDL/SDL.h>

#define SCREEN_WIDTH		(320)
#define SCREEN_HEIGHT		(240)
#define SCREEN_BPP			(32)

#if defined(MIYOO)
#define KEY_A		SDLK_LALT
#define KEY_B		SDLK_LCTRL
#define KEY_X		SDLK_LSHIFT
#define KEY_Y		SDLK_SPACE
#define KEY_START	SDLK_RETURN
#define KEY_SELECT	SDLK_ESCAPE
#define KEY_L1		SDLK_TAB
#define KEY_R1		SDLK_BACKSPACE
#elif defined(RETROFW)
#define KEY_A		SDLK_LCTRL
#define KEY_B		SDLK_LALT
#define KEY_X		SDLK_SPACE
#define KEY_Y		SDLK_LSHIFT
#define KEY_START	SDLK_RETURN
#define KEY_SELECT	SDLK_ESCAPE
#define KEY_L1		SDLK_TAB
#define KEY_R1		SDLK_BACKSPACE
#elif defined(WEBOS)
#define KEY_OK		SDLK_RETURN
#define KEY_BACK	461
#define KEY_RED		403
#define KEY_GREEN	404
#define KEY_YELLOW	405
#define KEY_BLUE	406
#else
#define KEY_A		SDLK_LALT
#define KEY_B		SDLK_LCTRL
#define KEY_X		SDLK_LSHIFT
#define KEY_Y		SDLK_SPACE
#define KEY_START	SDLK_RETURN
#define KEY_SELECT	SDLK_ESCAPE
#define KEY_L1		SDLK_z
#define KEY_R1		SDLK_x
#endif

extern SDL_Surface *screen;
#endif
