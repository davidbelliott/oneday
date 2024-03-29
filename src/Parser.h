#pragma once
class World;
class Command;
class Object;
class GameState;
#include "Token.h"
#include "Directions.h"
#include <string>
#include <vector>

// Converts a string to lower case
std::string to_lower(std::string str);

// Converts a string to upper case
std::string to_upper(std::string str);

// Converts a string to a series of tokens
token_list tokenize(std::string str, char delim);

// Returns a slice of the token list from start to the end specified (or end of list if end == -1)
token_list slice(token_list list, int start, int end=-1);

// Removes all of the tokens_to_remove from tokens
void remove_tokens(token_list* tokens, token_list tokens_to_remove);

// Joins all of the tokens in the list together with the delim char in between
std::string join(token_list tokens, char delim);

// Attempts to match two tokens, pushing arguments to args if needed
bool match_tokens(token s_token, token p_token, arg_list* args);

// Attempts to match two lists of tokens, pushing arguments to args if needed
bool match_token_lists(token_list statement, token_list pattern, arg_list* args);

/* Clears args, tokenizes pattern and statement, and uses the two above functions to recursively test
 * for equality of the statement and pattern. */
bool matches(std::string statement, std::string pattern, arg_list& args);

// Iterates through the provided tokens until it is able to find one corresponding
// to an object in the gamestate provided. If index is not null, it will be set
// to the index of the token corresponding to the object (or -1 if object not found).
Object* get_object(token_list tokens, World* world, int* index = nullptr);

class Parser
{
public:
    token_list tokens_to_remove;
    std::vector<Command*> parsing_cmds;

public:
	Parser();
	virtual ~Parser();

    Command* parse(std::string statement, GameState* g);
};
