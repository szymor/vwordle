#include "definitions.hpp"
#include "global.hpp"
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
	if (!wrapped.eof())
	{
		temp.push_back(wrapped.str());
		wrapped.flush();
	}
	return temp;
}

void Defintions::GetDefinitionsAndOthers(std::string word)
{
	sqlite3* db;
	int exit = 0;
	exit = sqlite3_open(database_name.c_str(), &db);
	std::string data("CALLBACK FUNCTION");
	std::stringstream s;
	std::string winning_word = word;
	std::transform(winning_word.begin(), winning_word.end(), winning_word.begin(), tolower);
	s << "SELECT * FROM definitions where word=\"" << winning_word << "\";";
	std::string sql(s.str());
	if (exit)
	{
		std::cerr << "Error open DB " << sqlite3_errmsg(db) << std::endl;
	}
	else
	{
		std::cout << "Opened Database Successfully" << std::endl;
	}

	int rc = sqlite3_exec(db, sql.c_str(), sqlite3QuerySelectCallback, (void*)data.c_str(), NULL);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error SELECT " << std::endl;
	}
	else
	{
		std::cout << "Operation OK!" << std::endl;
	}
	sqlite3_close(db);
}

void Defintions::ShowDefinition(SDL_Surface& screen, TTF_FONT& font, SDL_Rect &dst)
{
	if (max_definitions != 0)
	{
		SDL_Surface* text = nullptr;
		text = TTF_RenderUTF8_Blended(font_bold, "definition", (SDL_Color) { 255, 255, 255 });
		dst.x = (SCREEN_WIDTH - text->w) / 2;
		dst.y += 20;
		SDL_BlitSurface(text, NULL, screen, &dst);
		SDL_FreeSurface(text);
		dst.x = 0;
		for (auto temp : RenderTextWrap(definitions.at(current_definition), 45))
		{
			text = TTF_RenderUTF8_Blended(font, temp.c_str(), (SDL_Color) { 255, 255, 255 });
			if (dst.x == 0)
				dst.x = (SCREEN_WIDTH - text->w) / 2;
			dst.y += 20;
			SDL_BlitSurface(text, NULL, screen, &dst);
			SDL_FreeSurface(text);
			temp.clear();
		}
		text = TTF_RenderUTF8_Blended(font_bold, "speech part", (SDL_Color) { 255, 255, 255 });
		dst.x = (SCREEN_WIDTH - text->w) / 2;
		dst.y += 20;
		SDL_BlitSurface(text, NULL, screen, &dst);
		SDL_FreeSurface(text);

		text = TTF_RenderUTF8_Blended(font, speech_parts.at(current_definition).c_str(), (SDL_Color) { 255, 255, 255 });
		dst.x = 150;
		dst.y += 20;
		SDL_BlitSurface(text, NULL, screen, &dst);
		SDL_FreeSurface(text);
	}
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
