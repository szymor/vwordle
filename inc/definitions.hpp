#include <SDL/SDL.h>
#include <string>
#include <iostream>
#include <vector>
class Defintions
{ 
private:
	std::string database_name = "dict/def/definitions.d";
	std::vector<std::string> definitions;
	std::vector<std::string> speech_parts;
	std::vector<std::string> synonyms;
	int max_definitions;
	int current_definition = 1;
	void GetDefinitionsAndOthers(std::string word);
	
	static int sqlite3QuerySelectCallback(void* data, int argc, char** argv, char** azColName)
	{
		fprintf(stderr, "%s: ", (const char*)data);

		if(azColName == "def1")
			std::count << ""
		return 0;
	}

public:
	void ShowDefinition(SDL_Surface* screen, TTF_FONT* font);
	void SetNextDefinition();
	void SetPreviousDefinition();
	std::vector<std::string> RenderTextWrap(std::string str, int pos);
};