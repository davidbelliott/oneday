#pragma once
class World;
#include "Command.h"
#include "Word.h"
#include "WordList.h"
#include <string>

class Parser
{
public:
	WordList word_list;
public:
	Parser();
	virtual ~Parser();

    static std::vector<std::string> tokenize(std::string input, char delim = ' ');
    static cmd_ptr get_cmd(std::string word, World* w);
    Object* get_object(std::string name, World* w);
    cmd_ptr parse(std::string statement, World* w);
};

