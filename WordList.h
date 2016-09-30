#pragma once

#include "Word.h"
#include <map>
#include <string>

class WordList
{
public:
	std::map<std::string, Word> words_by_name;
	std::map<int, Word> words_by_id;

	const int //Actions
		GO,
		LOOK,
		QUIT,
		TAKE,
		WEAR,
		THROW,
		HIT,
		OPEN_CONTAINER,
		OPEN_DOOR,
		READ,
		DROP,
		PUT_IN_CONTAINER,
		TURN_ON,
		TURN_OFF,
		MOVE,
		INVENTORY,
		EAT,
		DRINK,
		SHOUT,
		CLOSE_DOOR,
		BREAK,
		BLESSUP,

		//Action modifiers
		NORTH,
		EAST,
		SOUTH,
		WEST,
		UP,
		DOWN;
		
	int N_WORDS;

	WordList();
	~WordList();

	Word get_word(std::string name);

private:
	int add_word(Word word);
};

