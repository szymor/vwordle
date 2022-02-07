#ifndef _HPP_STATES
#define _HPP_STATES

#include <string>
#include <SDL/SDL.h>

#define WORD_SIZE				(5)
#define MAX_WRONG_GUESSES		(6)

enum StateId
{
	SI_MENU,
	SI_GAME,
	SI_QUIT
};

class State
{
public:
	virtual void draw() = 0;
	virtual void processInput() = 0;
};

class GameState : public State
{
private:
	enum BoxType
	{
		BT_WRONG,
		BT_LETTER_OK,
		BT_LETTER_POSITION_OK,
		BT_ACTIVE
	};

	SDL_Surface *bg = nullptr;
	SDL_Surface *gfx = nullptr;
	char letters[MAX_WRONG_GUESSES][WORD_SIZE];
	BoxType bts[MAX_WRONG_GUESSES][WORD_SIZE];
	int wrong_guesses = 0;
	int active_letter = 0;
	std::string word_to_guess = "SKILL";

	void drawLetter(int x, int y, char c);
	void drawBox(int x, int y, BoxType bt);
public:
	void loadGfx();
	void unloadGfx();
	void resetGame();
	virtual void draw();
	virtual void processInput();
};

class MenuState : public State
{
public:
	virtual void draw();
	virtual void processInput();
};

extern StateId stateid;
extern GameState gamestate;
extern MenuState menustate;

#endif
