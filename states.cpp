#include "states.hpp"
#include "global.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <map>
#include <SDL/SDL_image.h>

#define SPRITE_SIZE			(40)
#define MAX_ENTRY_ID		(1)
#define GFX_PATH			"gfx/"
#define DICT_PATH			"dict/"

StateId stateid = SI_MENU;
GameState gamestate;
MenuState menustate;
RulesState rulesstate;

void State::processAll()
{
	while (getMyStateId() == stateid)
	{
		draw();
		processInput();
	}
}

StateId GameState::getMyStateId()
{
	return SI_GAME;
}

void GameState::drawLetter(int x, int y, char c)
{
	if (c < 'A' || c > 'Z')
		return;
	SDL_Rect src, dst;
	src.w = SPRITE_SIZE;
	src.h = SPRITE_SIZE;
	src.y = 0;
	src.x = SPRITE_SIZE * (4 + (c - 'A'));
	dst.x = x;
	dst.y = y;
	SDL_BlitSurface(gfx, &src, screen, &dst);
}

void GameState::drawBox(int x, int y, BoxType bt)
{
	SDL_Rect src, dst;
	src.w = SPRITE_SIZE;
	src.h = SPRITE_SIZE;
	src.y = 0;
	src.x = (int)bt * SPRITE_SIZE;
	dst.x = x;
	dst.y = y;
	SDL_BlitSurface(gfx, &src, screen, &dst);
}

void GameState::loadGfx()
{
	bg = IMG_Load(GFX_PATH "bg.png");
	gfx = IMG_Load(GFX_PATH "gfx.png");
	unknown_word_dialog = IMG_Load(GFX_PATH "unknownword.png");
	win_dialog = IMG_Load(GFX_PATH "won.png");
	lose_dialog = IMG_Load(GFX_PATH "lost.png");
}

void GameState::unloadGfx()
{
	SDL_FreeSurface(bg);
	bg = nullptr;
	SDL_FreeSurface(gfx);
	gfx = nullptr;
	SDL_FreeSurface(unknown_word_dialog);
	unknown_word_dialog = nullptr;
	SDL_FreeSurface(win_dialog);
	win_dialog = nullptr;
	SDL_FreeSurface(lose_dialog);
	lose_dialog = nullptr;
}

void GameState::loadDictionary(int letternum)
{
	std::string path = DICT_PATH + std::to_string(letternum) + ".txt";
	word_size = letternum;
	std::ifstream in(path);
	std::string word;
	dict.clear();
	while (in.good())
	{
		in >> word;
		if (word.size() != word_size)
		{
			std::cerr << "Dictionary error: " << word << std::endl;
			continue;
		}
		std::transform(word.begin(), word.end(), word.begin(), toupper);
		dict.insert(word);
	}
	std::cout << "Loaded " << dict.size() << " words." << std::endl;
}

void GameState::resetGame()
{
	for (int j = 0; j < MAX_WRONG_GUESSES; ++j)
		for (int i = 0; i < MAX_WORD_SIZE; ++i)
		{
			letters[j][i] = ' ';
			bts[j][i] = BT_WRONG;
		}
	wrong_guesses = 0;
	active_letter = 0;
	gamestatus = GS_INPROGRESS;
	yellows.clear();

	// choose a random word
	int steps = rand() % dict.size();
	auto it = dict.begin();
	std::advance(it, steps);
	word_to_guess = *it;
	std::cout << "Word to guess: " << word_to_guess << std::endl;
}

void GameState::draw()
{
	SDL_BlitSurface(bg, NULL, screen, NULL);
	for (int j = 0; j < MAX_WRONG_GUESSES; ++j)
		for (int i = 0; i < word_size; ++i)
		{
			int x = (SCREEN_WIDTH - word_size * SPRITE_SIZE) / 2 + i * SPRITE_SIZE;
			int y = j * SPRITE_SIZE;
			if (j == wrong_guesses && i == active_letter)
				drawBox(x, y, BT_ACTIVE);
			else
				drawBox(x, y, bts[j][i]);
			drawLetter(x, y, letters[j][i]);
		}
	if (GS_UNKNOWN_WORD == gamestatus)
		SDL_BlitSurface(unknown_word_dialog, NULL, screen, NULL);
	else if (GS_WON == gamestatus)
		SDL_BlitSurface(win_dialog, NULL, screen, NULL);
	else if (GS_LOST == gamestatus)
		SDL_BlitSurface(lose_dialog, NULL, screen, NULL);
	SDL_Flip(screen);
}

void GameState::verifyInputWord()
{
	// check if in the dictionary
	std::string word = "";
	for (int i = 0; i < word_size; ++i)
	{
		word += letters[wrong_guesses][i];
	}
	if (dict.count(word) == 0)
	{
		gamestatus = GS_UNKNOWN_WORD;
	}
	else
	{
		// search for green guesses
		int correct_letters = 0;
		std::map<char, int> chars;
		for (int i = 0; i < word_size; ++i)
		{
			if (word_to_guess[i] == letters[wrong_guesses][i])
			{
				bts[wrong_guesses][i] = BT_LETTER_POSITION_OK;
				++correct_letters;
			}
			else
			{
				++chars[word_to_guess[i]];
			}
		}
		if (word_size == correct_letters)
		{
			wrong_guesses = MAX_WRONG_GUESSES; // do not display the red box
			gamestatus = GS_WON;
		}
		else
		{
			// search for yellow guesses
			for (int i = 0; i < word_size; ++i)
			{
				if (chars[letters[wrong_guesses][i]] > 0 && bts[wrong_guesses][i] != BT_LETTER_POSITION_OK)
				{
					bts[wrong_guesses][i] = BT_LETTER_OK;
					--chars[letters[wrong_guesses][i]];
					yellows.insert(letters[wrong_guesses][i]);
				}
			}
			++wrong_guesses;
			if (MAX_WRONG_GUESSES == wrong_guesses)
			{
				gamestatus = GS_LOST;
			}
			active_letter = 0;
		}
	}
}

void GameState::processInput()
{
	SDL_Event event;
	bool leave = false;
	if (SDL_WaitEvent(&event) && !leave)
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_LEFT:
					{
						if (GS_INPROGRESS == gamestatus)
						{
							if (active_letter > 0)
							{
								--active_letter;
								leave = true;
							}
						}
					} break;
					case SDLK_RIGHT:
					{
						if (GS_INPROGRESS == gamestatus)
						{
							if (active_letter < (word_size - 1))
							{
								++active_letter;
								leave = true;
							}
						}
					} break;
					case SDLK_UP:
					{
						if (GS_INPROGRESS == gamestatus)
						{
							char &letter = letters[wrong_guesses][active_letter];
							if (letter < 'A' || letter > 'Z')
								letter = 'Z';
							else
								--letter;
							leave = true;
						}
					} break;
					case SDLK_DOWN:
					{
						if (GS_INPROGRESS == gamestatus)
						{
							char &letter = letters[wrong_guesses][active_letter];
							if (letter < 'A' || letter > 'Z')
								letter = 'A';
							else
								++letter;
							leave = true;
						}
					} break;
					case SDLK_RETURN:
					{
						if (GS_UNKNOWN_WORD == gamestatus)
						{
							active_letter = 0;
							gamestatus = GS_INPROGRESS;
						}
						else if (GS_WON == gamestatus)
						{
							stateid = SI_MENU;
						}
						else if (GS_LOST == gamestatus)
						{
							stateid = SI_MENU;
						}
						leave = true;
					} break;
					case SDLK_LALT:
					{
						if (GS_INPROGRESS == gamestatus)
						{
							verifyInputWord();
						}
					} break;
					case SDLK_LCTRL:
					{
						if (GS_INPROGRESS == gamestatus)
						{
							if (wrong_guesses > 0)
							{
								letters[wrong_guesses][active_letter] = letters[wrong_guesses - 1][active_letter];
								leave = true;
							}
						}
					} break;
					case SDLK_SPACE:
					{
						if (GS_INPROGRESS == gamestatus)
						{
							// find the next yellow letter if any
							auto it = yellows.find(letters[wrong_guesses][active_letter]);
							if (it != yellows.end())
							{
								++it;
								if (it == yellows.end())
									it = yellows.begin();
							}
							else
							{
								it = yellows.begin();
							}
							// if any successor exists, assign it
							if (it != yellows.end())
							{
								letters[wrong_guesses][active_letter] = *it;
								leave = true;
							}
						}
					} break;
					case SDLK_ESCAPE:
					{
						stateid = SI_MENU;
						leave = true;
					} break;
				} break;
			case SDL_QUIT:
			{
				stateid = SI_QUIT;
				leave = true;
			} break;
		}
	}
}

int MenuState::getWordLength()
{
	return word_length;
}

StateId MenuState::getMyStateId()
{
	return SI_MENU;
}

void MenuState::loadGfx()
{
	bg = IMG_Load(GFX_PATH "bgmenu.png");
	fg = IMG_Load(GFX_PATH "fgmenu.png");
	digit_select = IMG_Load(GFX_PATH "digit_selection.png");
	digit_select_white = IMG_Load(GFX_PATH "digit_selection_white.png");
	rules_select = IMG_Load(GFX_PATH "rules_selection.png");
}

void MenuState::unloadGfx()
{
	SDL_FreeSurface(bg);
	bg = nullptr;
	SDL_FreeSurface(fg);
	fg = nullptr;
	SDL_FreeSurface(digit_select);
	digit_select = nullptr;
	SDL_FreeSurface(digit_select_white);
	digit_select_white = nullptr;
	SDL_FreeSurface(rules_select);
	rules_select = nullptr;
}

void MenuState::draw()
{
	SDL_Rect dst;
	SDL_BlitSurface(bg, NULL, screen, NULL);
	if (0 == index)
	{
		dst.x = 171 + 24 * (word_length - MIN_WORD_SIZE);
		dst.y = 128;
		SDL_BlitSurface(digit_select, NULL, screen, &dst);
	}
	else if (1 == index)
	{
		dst.x = 171 + 24 * (word_length - MIN_WORD_SIZE);
		dst.y = 128;
		SDL_BlitSurface(digit_select_white, NULL, screen, &dst);
		dst.x = 97;
		dst.y = 144;
		SDL_BlitSurface(rules_select, NULL, screen, &dst);
	}
	dst.x = 38;
	dst.y = 150;
	SDL_BlitSurface(fg, NULL, screen, &dst);
	SDL_Flip(screen);
}

void MenuState::processInput()
{
	SDL_Event event;
	bool leave = false;
	if (SDL_WaitEvent(&event) && !leave)
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_LEFT:
					{
						if (0 == index)
						{
							if (word_length > MIN_WORD_SIZE)
							{
								--word_length;
								leave = true;
							}
						}
					} break;
					case SDLK_RIGHT:
					{
						if (0 == index)
						{
							if (word_length < MAX_WORD_SIZE)
							{
								++word_length;
								leave = true;
							}
						}
					} break;
					case SDLK_UP:
					{
						if (index > 0)
						{
							--index;
							leave = true;
						}
					} break;
					case SDLK_DOWN:
					{
						if (index < MAX_ENTRY_ID)
						{
							++index;
							leave = true;
						}
					} break;
					case SDLK_RETURN:
					{
						if (0 == index)
							stateid = SI_GAME;
						else if (1 == index)
							stateid = SI_RULES;
						leave = true;
					} break;
					case SDLK_ESCAPE:
					{
						stateid = SI_QUIT;
						leave = true;
					} break;
				} break;
			case SDL_QUIT:
			{
				stateid = SI_QUIT;
				leave = true;
			} break;
		}
	}
}

StateId RulesState::getMyStateId()
{
	return SI_RULES;
}

void RulesState::loadGfx()
{
	bg = IMG_Load(GFX_PATH "bgrules.png");
}

void RulesState::unloadGfx()
{
	SDL_FreeSurface(bg);
	bg = nullptr;
}

void RulesState::draw()
{
	SDL_BlitSurface(bg, NULL, screen, NULL);
	SDL_Flip(screen);
}

void RulesState::processInput()
{
	SDL_Event event;
	bool leave = false;
	if (SDL_WaitEvent(&event) && !leave)
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_RETURN:
					case SDLK_ESCAPE:
					{
						stateid = SI_MENU;
						leave = true;
					} break;
				} break;
			case SDL_QUIT:
			{
				stateid = SI_QUIT;
				leave = true;
			} break;
		}
	}
}
