#include "definitions.hpp"
#include <sstream>
#include <SDL/SDL_ttf.h>
std::vector<std::string> Defintions::RenderTextWrap(std::string str, int pos)
{
	std::istringstream words(str);
	std::ostringstream wrapped;
	std::vector<std::string> temp;
	std::string word;
	if (words >> word)
	{
		wrapped << word;
		size_t space_left = pos - word.length();
		while (words >> word)
		{
			if (space_left < word.length() + 1)
			{
				wrapped << word;
				temp.push_back(wrapped.str());
				wrapped.flush();
				space_left = pos - word.length();
			}
			else
			{
				wrapped << ' ' << word;
				space_left -= word.length() + 1;
			}
		}
	}
	temp.push_back(wrapped.str());
	wrapped.flush();
	return temp;
}

void Defintions::GetDefinitionsAndOthers(std::string word)
{
	
}

void Defintions::ShowDefinition(SDL_Surface& screen, TTF_FONT& font, SDL_Rect &dst)
{
	SDL_Surface* text = nullptr;
	text = TTF_RenderUTF8_Blended(font_bold, "definition", (SDL_Color) { 255, 255, 255 });
	dst.x = (SCREEN_WIDTH - text->w) / 2;
	dst.y += 20;
	SDL_BlitSurface(text, NULL, screen, &dst);
	SDL_FreeSurface(text);
	text = TTF_RenderUTF8_Blended(font, temp.c_str(), (SDL_Color) { 255, 255, 255 });
	dst.x = (SCREEN_WIDTH - text->w) / 2;
	dst.y += 20;
	SDL_BlitSurface(text, NULL, screen, &dst);
	SDL_FreeSurface(text);
	temp.clear();
		}
		else
		{
			temp.push_back(ch);
		}
	}
	text = TTF_RenderUTF8_Blended(font, temp.c_str(), (SDL_Color) { 255, 255, 255 });
	dst.x = (SCREEN_WIDTH - text->w) / 2;
	dst.y += 20;
	temp.clear();
	SDL_BlitSurface(text, NULL, screen, &dst);
	SDL_FreeSurface(text);

	text = TTF_RenderUTF8_Blended(font_bold, "speech part", (SDL_Color) { 255, 255, 255 });
	dst.x = (SCREEN_WIDTH - text->w) / 2;
	dst.y += 20;
	SDL_BlitSurface(text, NULL, screen, &dst);
	SDL_FreeSurface(text);

	text = TTF_RenderUTF8_Blended(font, speech_part_first.c_str(), (SDL_Color) { 255, 255, 255 });
	dst.x = 150;
	dst.y += 20;
	SDL_BlitSurface(text, NULL, screen, &dst);
	SDL_FreeSurface(text);
}

void Defintions::SetNextDefinition()
{
	if (current_definition < max_definitions)
		current_definition++; 
}

void Defintions::SetPreviousDefinition()
{
	if (current_definition > 1)
		current_definition--;
}
