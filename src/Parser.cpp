#include "Parser.h"
#include "Room.h"
#include "World.h"
#include "Player.h"

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

cmd_ptr Parser::get_cmd(Word word)
{
		/*if (word.id == l->GO) return new ActionGo(word);
		else if (word.id == l->LOOK) return new ActionLook(word);
		else if (word.id == l->QUIT) return new ActionQuit(word);
		else if (word.id == l->TAKE) return new ActionTake(word);
		else if (word.id == l->WEAR) return new ActionWear(word);
		else if (word.id == l->HIT) return new ActionHit(word);
		else if (word.id == l->OPEN_CONTAINER) return new ActionOpenContainer(word);
		else if (word.id == l->READ) return new ActionRead(word);
		else if (action_type == Action::PUT_IN_CONTAINER) return new ActionPutInContainer();
		else if (action_type == Action::TURN_ON) return new ActionTurnOn();
		else if (action_type == Action::TURN_OFF) return new ActionTurnOff();
		else if (action_type == Action::MOVE) return new ActionMove();
		else if (action_type == Action::EXAMINE) return new ActionExamine();
		else if (action_type == Action::INVENTORY) return new ActionInventory();
		else if (action_type == Action::EAT) return new ActionEat();
		else if (action_type == Action::DRINK) return new ActionDrink();
		else if (word.id == l->SHOUT) return new ActionShout(word);
		else if (action_type == Action::BREAK) return new ActionBreak();
		else if (action_type == Action::BLESSUP) return new ActionBlessup();
        else if (word.id == l->TALK_TO) return new ActionTalkTo(word);
        else if (word.id == l->HELP) return new ActionHelp(word);*/
}

Parser::Parser()
 : word_list()
{
}


Parser::~Parser()
{
}

bool matches_regex(std::string regex, std::string str)
{


}

cmd_ptr Parser::parse(std::string statement, World* w)
{
    cmd_ptr command = nullptr;
	Object* object = nullptr;
	Room* room = w->get_current_room();
	std::vector<std::string> token_strings = tokenize(statement, ' ');
	std::vector<Word> tokens;

	for (size_t i = 0; i < token_strings.size(); i++)
	{
		tokens.push_back(word_list.get_word(token_strings[i]));
	}

	bool found_command = false;
	bool found_object = false;
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].get_part_of_speech() == Word::ACTION && !found_command)
		{
			command = get_cmd(tokens[i]);
            if(command)
                found_command = true;
		}
		else if (!found_object && found_command)
		{
			if (object = room->get_indirect_child(tokens[i].word, Object::DISCOVERED))
			{
				found_object = true;
				command->add_object(object);
			}
            else if (object = w->player->get_indirect_child(tokens[i].word, Object::DISCOVERED))
            {
                found_object = true;
                command->add_object(object);
            }
		}
	}

	return command;
}
