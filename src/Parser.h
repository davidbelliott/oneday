#pragma once
class Receiver;
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

    static std::vector<std::string> tokenize(std::string input, char delim = ' ');
	Action* parse(std::string statement, World* w, Receiver* r);
};

