#include "definitions.hpp"
#include "global.hpp"
#include <sstream>
#include <string>
#include <algorithm>
#include <sqlite3.h>

std::vector<std::string> definitions_temp;
std::vector<std::string> speech_parts_temp;
std::vector<std::string> synonyms_temp;
std::vector<std::string> Definitions::RenderTextWrap(std::string str, int pos)
{
	std::istringstream words(str);
	std::string wrapped = "";
	std::vector<std::string> temp;
	std::string word;
	if (words >> word)
	{
		wrapped += word;
		size_t space_left = pos - word.length();
		while (words >> word)
		{
			if (space_left < word.length()+1)
			{
				temp.push_back(wrapped);
				wrapped.clear();
				wrapped += word;
				space_left = pos - word.length();
			}
			else
			{
				wrapped +=  ' ' + word;
				space_left -= word.length()+1;
			}
		}
	}
	if (!wrapped.empty())
	{
		temp.push_back(wrapped);
		wrapped.clear();
	}
	return temp;
}

void Definitions::clearStores()
{
	definitions.empty();
	definitions.clear();
	speech_parts.clear();
	synonyms.clear();
}

int Definitions::GetMaxDefinitionsNumber()
{
	return max_definitions;
}

std::string Definitions::GetSpeechPartForWordDefinition()
{
	if (!speech_parts.empty())
	{
		return speech_parts.at(current_definition);
	}
	return " ";
}

std::string Definitions::GetSynonymsForWordDefinition()
{
	std::stringstream temp;
	if (!synonyms.empty())
	{ 
		int i = 0;
		for (auto synonym : synonyms)
		{
			if (i == 5)
				break;
			if(synonym != "")
				temp << synonym << ", ";
			i++;
		}
		std::string result = temp.str();
		result.pop_back();
		result.pop_back();
		return result;
	}
	return " ";
}

std::string Definitions::GetCurrentDefinition()
{
	if (!definitions.empty())
	{
		return definitions.at(current_definition);
	}
	return " ";
}


int Definitions::getCurrentDefinitionId()
{
	return current_definition;
}
static int sqlite3QuerySelectCallback(void* data, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(azColName[i], "definition") == 0)
		{
			definitions_temp.push_back(argv[i]);
		}
		if (strcmp(azColName[i], "speech_part") == 0)
		{
			speech_parts_temp.push_back(argv[i]);
		}
		if (strcmp(azColName[i], "synonym") == 0)
		{
			synonyms_temp.push_back(argv[i]);
		}
	}
	return 0;
}

void Definitions::GetDefinitionsAndOthers(std::string word)
{
	sqlite3* db;
	int exit = 0;
	exit = sqlite3_open(database_name.c_str(), &db);
	std::string data("CALLBACK FUNCTION");
	std::string winning_word = word;
	std::stringstream s;
	std::transform(winning_word.begin(), winning_word.end(), winning_word.begin(), tolower);
	s << "SELECT definitions.definition, speech_part.speech_part, synonyms.synonym FROM \
	definitions inner join speech_part on definitions.speech_part_id=speech_part.speechpart_id \
	inner join words on definitions.word_id=words.id inner join synonyms on definitions.definitions_id= \
	synonyms.synonym_id where words.word='" << winning_word << "'";
	if (exit)
	{
		std::cerr << "Error open DB " << sqlite3_errmsg(db) << std::endl;
	}
	else
	{
		std::cout << "Opened Database Successfully" << std::endl;
	}
	int rc = sqlite3_exec(db, s.str().c_str(), sqlite3QuerySelectCallback, (void*)data.c_str(), NULL);
	s.clear();
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error SELECT " << std::endl;
	}
	else
	{
		std::cout << "Operation OK!" << std::endl;
		clearStores();
		std::copy(definitions_temp.begin(), definitions_temp.end(), std::back_inserter(definitions));
		std::copy(speech_parts_temp.begin(), speech_parts_temp.end(), std::back_inserter(speech_parts));
		std::copy(synonyms_temp.begin(), synonyms_temp.end(), std::back_inserter(synonyms));
		definitions_temp.clear();
		speech_parts_temp.clear();
		synonyms_temp.clear();
	}
	max_definitions = definitions.size();
	current_definition = 0; 
	sqlite3_close(db);
}


void Definitions::SetNextDefinition()
{
	if (max_definitions != 0)
	{
		if (current_definition < max_definitions - 1)
			current_definition++;
	}
}

void Definitions::SetPreviousDefinition()
{
	if (current_definition > 0)
		current_definition--;
}
