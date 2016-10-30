#include "Parser.h"
#include "Player.h"
#include "Room.h"
#include "Engine.h"
#include "Word.h"
#include "World.h"
#include "CmdDisp.h"

#include <vector>

std::vector<std::string> Parser::tokenize(std::string input, char delim)
{
	std::vector<std::string> tokens;
	std::string cur_str = "";
	for (size_t i = 0; i < input.size(); i++)
	{
		if (input[i] == delim)
		{
			if (cur_str.size() > 0)
			{
				tokens.push_back(cur_str);
				cur_str = "";
			}
		}
		else
			cur_str.push_back(input[i]);
	}
	if (cur_str.size() > 0)
		tokens.push_back(cur_str);
	return tokens;
}

Parser::Parser()
 : action_factory(), word_list()
{
}


Parser::~Parser()
{
}

bool matches_regex(std::string regex, std::string str)
{


}

Action* Parser::parse(std::string statement, World* w, Engine* e)
{
	Action* action = NULL;
	Object* object = NULL;
	Room* room = w->get_current_room();
	std::vector<std::string> token_strings = tokenize(statement, ' ');
	std::vector<Word> tokens;

	for (size_t i = 0; i < token_strings.size(); i++)
	{
		tokens.push_back(word_list.get_word(token_strings[i]));
	}

	bool found_action = false;
	bool found_object = false;
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].get_part_of_speech() == Word::ACTION && !found_action)
		{
			action = action_factory.create_action(tokens[i]);
            if(action)
                found_action = true;
		}
		else if (tokens[i].get_part_of_speech() == Word::ACTION_MODIFIER && found_action)
		{
			action->add_preposition(tokens[i]);
		}
		else if (!found_object && found_action)
		{
			if (object = room->get_indirect_child(tokens[i].word, Object::DISCOVERED))
			{
				found_object = true;
				action->add_object(object);
			}
            else if (object = w->player->get_indirect_child(tokens[i].word, Object::DISCOVERED))
            {
                found_object = true;
                action->add_object(object);
            }
		}
	}
	if (!found_action)
	{
		if (statement.empty())
			e->push_event(new CmdDisp("Please enter a command."));
		else
			e->push_event(new CmdDisp("I don't understand."));
	}

	return action;
}
