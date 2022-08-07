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
	definitions.clear();
	speech_parts.clear();
	synonyms.clear();
}

static int sqlite3QuerySelectCallback(void* data, int argc, char** argv, char** azColName)
{
	const char* option = (const char*)data;
	if (strcmp(option, "def") == 0)
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
		}
	}
	else if (strcmp(option, "syn") == 0)
	{
		for (int i = 0; i < argc; i++)
		{
			if (strcmp(azColName[i], "word") == 0)
			{
				synonyms_temp.push_back(argv[i]);
			}
		}
	}
	return 0;
}


std::string Definitions::GetSynonyms(std::string definition)
{
	int exit = 0;
	sqlite3* db;
	synonyms_temp.clear();
	exit = sqlite3_open(database_name.c_str(), &db);
	if (exit)
	{
		std::cerr << "Error to open DB: " << sqlite3_errmsg(db) << "\n";
		return "";
	}
	else
	{
		std::string sql = "select words.word from definitions inner join words on definitions.word_id=words.id where definitions.definition='" + definition + "'";
		int rc = sqlite3_exec(db, sql.c_str(), sqlite3QuerySelectCallback, (void*)"syn", NULL);
		if (rc != SQLITE_OK)
		{
			std::cerr << "Error SELECT synonyms " << std::endl;
			sqlite3_close(db);
			return "";
		}
		else
		{
			std::string temp = "";
			if (!synonyms.empty())
			{
				for (auto synonym : synonyms_temp)
				{
					if (synonym == winning_word)
						continue;
					temp += synonym + std::string(", ");
				}
				synonyms_temp.clear();
				if (!temp.empty())
				{
					temp.pop_back();
					temp.pop_back();
				}
			}
			sqlite3_close(db);
			return temp;
		}
	}
	sqlite3_close(db);
	return "";
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
	return "";
}


std::string Definitions::GetSynonymsForWordDefinition()
{
	if (!synonyms.empty())
	{
		return synonyms.at(current_definition);
	}
	return "";
}

std::string Definitions::GetCurrentDefinition()
{
	if (!definitions.empty())
	{
		return definitions.at(current_definition);
	}
	return "";
}


int Definitions::GetCurrentDefinitionId()
{
	return current_definition;
}


void Definitions::GetDefinitionsAndOthers(std::string word)
{
	int exit = 0;
	sqlite3* db;
	exit = sqlite3_open(database_name.c_str(), &db);
	std::string option = "def";
	std::stringstream s;
	winning_word = word;
	std::transform(winning_word.begin(), winning_word.end(), winning_word.begin(), tolower);
	s << "SELECT definitions.definition, speech_part.speech_part FROM \
	definitions inner join speech_part on definitions.speech_part_id=speech_part.speechpart_id \
	inner join words on definitions.word_id=words.id where words.word='" << winning_word << "'";
	if (exit)
	{
		std::cerr << "Error open DB " << sqlite3_errmsg(db) << std::endl;
	}
	else
	{
		std::cout << "Opened Database Successfully" << std::endl;

		int rc = sqlite3_exec(db, s.str().c_str(), sqlite3QuerySelectCallback, (void*)option.c_str(), NULL);
		s.clear();
		if (rc != SQLITE_OK)
		{
			std::cerr << "Error SELECT definitions " << std::endl;
		}
		else
		{
			clearStores();
			std::copy(definitions_temp.begin(), definitions_temp.end(), std::back_inserter(definitions));
			std::copy(speech_parts_temp.begin(), speech_parts_temp.end(), std::back_inserter(speech_parts));
			definitions_temp.clear();
			speech_parts_temp.clear();
			if (!definitions.empty())
			{
				for (auto def : definitions)
				{
					std::string temp = GetSynonyms(def);
					if(!temp.empty())
						synonyms.push_back(temp);
					else
					{
						synonyms.push_back(" ");
					}
				}
			}
		}
		max_definitions = definitions.size();
		current_definition = 0;
		sqlite3_close(db);
	}
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
