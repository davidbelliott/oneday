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

cmd_ptr Parser::get_cmd(std::string word, World* world)
{
    cmd_ptr cmd = nullptr;
    if(word == "look")
    {
    }
    else if (word == "quit" || word == "q") cmd = std::make_shared<CmdQuit>();

    
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

    return cmd;
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

Object* Parser::get_object(std::string name, World* w)
{
    Object* object = nullptr;
    Room* room = w->get_current_room();
    if (object = room->get_indirect_child(name, Object::DISCOVERED))
    {
        return object;
    }
    else if (object = w->player->get_indirect_child(name, Object::DISCOVERED))
    {
        return object;
    }
    return object;
}

cmd_ptr Parser::parse(std::string statement, World* w)
{
	Object* object = nullptr;
	Room* room = w->get_current_room();
	std::vector<std::string> tokens = tokenize(statement, ' ');

	/*for (size_t i = 0; i < token_strings.size(); i++)
	{
		tokens.push_back(word_list.get_word(token_strings[i]));
	}*/

	for (size_t i = 0; i < tokens.size(); i++)
	{
        if(tokens[i] == "look")
        {
            if(tokens.size() > i + 1)
            {
                if(tokens[i + 1] == "around")
                {
                    cmd_ptr cmd = std::make_shared<CmdDescribe>();
                    cmd->add_object(w->get_current_room());
                    return cmd;
                }
                else if(tokens[i + 1] == "at")
                {
                    if(tokens.size() > i + 2)
                    {
                        Object* object = get_object(tokens[i + 2], w);
                        if(object)
                        {
                            cmd_ptr cmd = std::make_shared<CmdDescribe>();
                            cmd->add_object(object);
                            return cmd;
                        }
                    }
                }
            }
            return std::make_shared<CmdDisp>("Look at what?");
        }
        else if(tokens[i] == "go")
        {
            DirectionId direction = DIRECTION_MAX;
            if(tokens.size() > i + 1)
            {
                for(int j = 0; j < DIRECTION_MAX && direction == DIRECTION_MAX; j++)
                {
                    if(tokens[i + 1] == dir[(DirectionId)j].name)
                        direction = (DirectionId)j;
                }
            }

            if(direction == DIRECTION_MAX)
            {
                return std::make_shared<CmdDisp>("Go where?");
            }
            else
            {
                Room* room = w->get_current_room();
                if(room && room->directions[direction] != "") {
                    return std::make_shared<CmdSetRoom>(room->directions[direction]);
                } else {
                    return std::make_shared<CmdDisp>("You can't go " + dir[direction].name + " from here.");
                }
            }
        }
        else if(tokens[i] == "quit" || tokens[i] == "q")
        {
            return std::make_shared<CmdQuit>();
        }

        /*
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
		}*/
	}
    return nullptr;
}
