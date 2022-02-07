#include "states.hpp"
#include "global.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <map>
#include <SDL/SDL_image.h>

#define SPRITE_SIZE		(40)

StateId stateid = SI_GAME;
GameState gamestate;
MenuState menustate;

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
	bg = IMG_Load("bg.png");
	gfx = IMG_Load("gfx.png");
}

void GameState::unloadGfx()
{
	SDL_FreeSurface(bg);
	bg = nullptr;
	SDL_FreeSurface(gfx);
	gfx = nullptr;
}

void GameState::loadDictionary()
{
	std::ifstream in("5.txt");
	std::string word;
	while (in.good())
	{
		in >> word;
		if (word.size() != WORD_SIZE)
		{
			std::cerr << "Dictionary error: " << word << std::endl;
			continue;
		}
		std::transform(word.begin(), word.end(), word.begin(), toupper);
		dict.insert(word);
	}
}

void GameState::resetGame()
{
	for (int j = 0; j < MAX_WRONG_GUESSES; ++j)
		for (int i = 0; i < WORD_SIZE; ++i)
		{
			letters[j][i] = ' ';
			bts[j][i] = BT_WRONG;
		}

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
		for (int i = 0; i < WORD_SIZE; ++i)
		{
			int x = (SCREEN_WIDTH - WORD_SIZE * SPRITE_SIZE) / 2 + i * SPRITE_SIZE;
			int y = j * SPRITE_SIZE;
			if (j == wrong_guesses && i == active_letter)
				drawBox(x, y, BT_ACTIVE);
			else
				drawBox(x, y, bts[j][i]);
			drawLetter(x, y, letters[j][i]);
		}
	SDL_Flip(screen);
}

void GameState::verifyInputWord()
{
	// check if in the dictionary
	std::string word = "";
	for (int i = 0; i < WORD_SIZE; ++i)
	{
		word += letters[wrong_guesses][i];
	}
	if (dict.count(word) == 0)
	{
		for (int i = 0; i < WORD_SIZE; ++i)
		{
			letters[wrong_guesses][i] = ' ';
		}
		active_letter = 0;
		std::cout << "The given word is not in the dictionary." << std::endl;
	}
	else
	{
		// search for green guesses
		int correct_letters = 0;
		std::map<char, int> chars;
		for (int i = 0; i < WORD_SIZE; ++i)
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
		if (WORD_SIZE == correct_letters)
		{
			wrong_guesses = MAX_WRONG_GUESSES; // do not display the red box
			std::cout << "You won." << std::endl;
			gamestatus = GS_WON;
		}
		else
		{
			// search for yellow guesses
			for (int i = 0; i < WORD_SIZE; ++i)
			{
				if (chars[letters[wrong_guesses][i]] > 0 && bts[wrong_guesses][i] != BT_LETTER_POSITION_OK)
				{
					bts[wrong_guesses][i] = BT_LETTER_OK;
					--chars[letters[wrong_guesses][i]];
				}
			}
			++wrong_guesses;
			if (MAX_WRONG_GUESSES == wrong_guesses)
			{
				std::cout << "You lost." << std::endl;
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
							if (active_letter < (WORD_SIZE - 1))
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
						if (GS_INPROGRESS == gamestatus)
						{
							verifyInputWord();
						}
						leave = true;
					} break;
					case SDLK_RSHIFT:
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

void MenuState::draw()
{
}

void MenuState::processInput()
{
}
