#pragma once
class World;
#include "Token.h"
#include "Command.h"
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

/* Clears args, tokenizes statement and pattern, and uses the two above functions to recursively test
 * for equality of the statement and pattern. */
bool matches(std::string statement, std::string pattern, arg_list& args);

// Iterates through the provided tokens until it is able to find one corresponding
// to an object in the gamestate provided.
Object* get_object(token_list tokens, GameState* g);

enum ParseOutcome
{
    SUCCESS,
    UNKNOWN_VERB,
    UNKNOWN
};

class Parser
{
public:
    token_list tokens_to_remove;

public:
	Parser();
	virtual ~Parser();

    std::vector<cmd_ptr> parse(std::string statement, GameState* g);
};

