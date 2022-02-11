#ifndef _HPP_STATES
#define _HPP_STATES

#include <string>
#include <set>
#include <SDL/SDL.h>

#define MIN_WORD_SIZE				(5)
#define MAX_WORD_SIZE				(8)
#define MAX_WRONG_GUESSES			(6)

enum StateId
{
	SI_MENU,
	SI_GAME,
	SI_RULES,
	SI_QUIT
};

class State
{
private:
	virtual StateId getMyStateId() = 0;
public:
	virtual void draw() = 0;
	virtual void processInput() = 0;
	void processAll();
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
	enum GameStatus
	{
		GS_INPROGRESS,
		GS_WON,
		GS_LOST,
		GS_UNKNOWN_WORD,
		GS_VIRTUAL_KEYBOARD
	};

	SDL_Surface *bg = nullptr;
	SDL_Surface *gfx = nullptr;
	SDL_Surface *unknown_word_dialog = nullptr;
	SDL_Surface *win_dialog = nullptr;
	SDL_Surface *lose_dialog = nullptr;
	SDL_Surface *letter_select = nullptr;
	SDL_Surface *check_select = nullptr;
	SDL_Surface *keyboard_bg = nullptr;
	SDL_Surface *keyboard_fg = nullptr;
	char letters[MAX_WRONG_GUESSES][MAX_WORD_SIZE];
	BoxType bts[MAX_WRONG_GUESSES][MAX_WORD_SIZE];
	int wrong_guesses = 0;
	int active_letter = 0;
	int word_size = 5;
	int keyx = 0;
	int keyy = 0;
	std::string word_to_guess;
	std::set<std::string> dict;
	std::set<char> yellows;		// used for quick input of yellow letters
	GameStatus gamestatus = GS_INPROGRESS;

	virtual StateId getMyStateId();
	void drawLetter(int x, int y, char c);
	void drawBox(int x, int y, BoxType bt);
	void verifyInputWord();
	void incrementActiveLetter();
	void decrementActiveLetter();
public:
	void loadGfx();
	void unloadGfx();
	void loadDictionary(int letternum);
	void resetGame();
	virtual void draw();
	virtual void processInput();
};

class MenuState : public State
{
private:
	SDL_Surface *bg = nullptr;
	SDL_Surface *fg = nullptr;
	SDL_Surface *digit_select = nullptr;
	SDL_Surface *digit_select_white = nullptr;
	SDL_Surface *rules_select = nullptr;
	int index = 0;
	int word_length = 5;
	virtual StateId getMyStateId();
public:
	int getWordLength();
	void loadGfx();
	void unloadGfx();
	virtual void draw();
	virtual void processInput();
};

class RulesState : public State
{
private:
	SDL_Surface *bg = nullptr;
	virtual StateId getMyStateId();
public:
	void loadGfx();
	void unloadGfx();
	virtual void draw();
	virtual void processInput();
};

extern StateId stateid;
extern GameState gamestate;
extern MenuState menustate;
extern RulesState rulesstate;

#endif