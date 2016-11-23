#pragma once
class World;
#include "Instruction.h"
#include "Token.h"
#include <string>
#include <vector>

token_list tokenize(std::string str, char delim);
token_list slice(token_list list, int start);
void remove_tokens(token_list* tokens, token_list tokens_to_remove);

bool match_tokens(token s_token, token p_token, arg_list* args);
bool match_token_lists(token_list statement, token_list pattern, arg_list* args);

class Parser
{
public:
    std::vector<InstructionPtr> instruction_lookup_table;
    token_list tokens_to_remove;

public:
	Parser();
	virtual ~Parser();

    Object* get_object(std::string name, World* w);
    InstructionPtr parse(std::string statement, GameState* g);
};

