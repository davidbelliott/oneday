#pragma once
class World;
#include "Instruction.h"
#include "Token.h"
#include <string>
#include <vector>

// Converts a string to lower case
std::string to_lower(std::string str);

// Converts a string to a series of tokens
token_list tokenize(std::string str, char delim);

// Returns a slice of the token list from start to the end of the list
token_list slice(token_list list, int start);

// Removes all of the tokens_to_remove from tokens
void remove_tokens(token_list* tokens, token_list tokens_to_remove);

// Joins all of the tokens in the list together with the delim char in between
std::string join(token_list tokens, char delim);

// Attempts to match two tokens, pushing arguments to args if needed
bool match_tokens(token s_token, token p_token, arg_list* args);

// Attempts to match two lists of tokens, pushing arguments to args if needed
bool match_token_lists(token_list statement, token_list pattern, arg_list* args);

enum ParseOutcome
{
    SUCCESS,
    UNKNOWN_VERB,
    UNKNOWN
};

class Parser
{
public:
    std::vector<InstructionPtr> instruction_lookup_table;
    token_list tokens_to_remove;

public:
	Parser();
	virtual ~Parser();

    InstructionPtr parse(std::string statement, GameState* g);
};

