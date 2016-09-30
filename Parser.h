#pragma once
class Terminal;
class World;
#include "ActionFactory.h"
#include "Word.h"
#include "WordList.h"
#include <string>

class Parser
{
public:
	ActionFactory action_factory;
	WordList word_list;
public:
	Parser();
	virtual ~Parser();

    std::string extract_word(std::string::iterator& iter)
	Action* parse(std::string statement, World* w, Terminal* t);
};

