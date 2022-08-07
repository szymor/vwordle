#include <SDL/SDL.h>
#include <string>
#include <iostream>
#include <vector>
class Definitions
{ 
private:
	std::string database_name = "dict/def/definitions.db";
	std::vector<std::string> definitions;
	std::vector<std::string> speech_parts;
	std::vector<std::string> synonyms;
	std::string winning_word;
	int max_definitions;
	int current_definition = 0;
	void clearStores();
	std::string GetSynonyms(std::string definition);

public:
	int GetMaxDefinitionsNumber();
	std::string GetSpeechPartForWordDefinition();
	std::string GetSynonymsForWordDefinition();
	std::string GetCurrentDefinition();
	int GetCurrentDefinitionId();
	void GetDefinitionsAndOthers(std::string word);
	void SetNextDefinition();
	void SetPreviousDefinition();
	std::vector<std::string> RenderTextWrap(std::string str, int pos);
};