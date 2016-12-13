#include "Parser.h"
#include "World.h"
#include "Player.h"
#include "GameState.h"
#include "GameStateMenu.h"
#include "Engine.h"
#include "Command.h"
#include <iostream>

Parser::Parser()
 : tokens_to_remove({})
{
    tokens_to_remove = {
        "the",
        "a",
        "and"
    };
}

Parser::~Parser()
{
}

Object* get_object(token_list tokens, GameState* g, int* index)
{
    Object* found_object = nullptr;
    int i = 0;
    for( ; i < tokens.size() && !found_object; i++)
    {
        found_object = g->world->get_current_room()->get_indirect_child(tokens[i], 0);
        if(found_object && (!found_object->active || !found_object->discovered))
            found_object = nullptr;
        if(!found_object)
        {
            found_object = g->world->get_player()->get_indirect_child(tokens[i], 0);
            if(found_object && (!found_object->active || !found_object->discovered))
                found_object = nullptr;
        }
    }
    i--;
    if(index)
    {
        if(!found_object)
            *index = -1;
        else
            *index = i;
    }
        
    return found_object;
}


std::string to_lower(std::string str)
{
    std::string lower_str;
    for(int i = 0; i < str.size(); i++)
        lower_str.push_back(std::tolower(str[i]));
    return lower_str;
}

token_list slice(token_list list, int start, int end)
{
    if(end == -1)
        end = list.size();
    token_list slice_list(list.begin() + start, list.begin() + end);
    return slice_list;
}

token_list tokenize(std::string str, char delim)
{
    token_list tokens;
    token cur_str = "";
    for(int i = 0; i < str.size(); i++)
    {
        if(str[i] == delim)
        {
            if(cur_str.size() > 0)
            {
                tokens.push_back(cur_str);
                cur_str = "";
            }
        }
        else
            cur_str.push_back(str[i]);
    }
    if(cur_str.size() > 0)
        tokens.push_back(cur_str);
    return tokens;
}

void remove_tokens(token_list* tokens, token_list tokens_to_remove)
{
    for(int i = 0; i < tokens_to_remove.size(); i++)
    {
        tokens->erase(std::remove(tokens->begin(), tokens->end(), tokens_to_remove[i]), tokens->end());
    }
}

std::string join(token_list list, char delim)
{
    std::string cur_str = "";
    for(int i = 0; i < list.size(); i++)
    {
        cur_str += list[i];
        if(i < list.size() - 1)
            cur_str += delim;
    }
    return cur_str;
}

bool match_tokens(token s_token, token p_token, arg_list* args)
{
    //std::cout << "Comparing token: " << s_token << " | " << p_token << std::endl;

    if(p_token != "#")
    {
        return (s_token == p_token);
    }
    else
    {
        args->push_front( { s_token } );
        return true;
    }
}

bool match_token_lists(token_list statement, token_list pattern, arg_list* args)
{
    /*std::cout << "Comparing token list:";
    for(int i = 0; i < statement.size(); i++)
        std::cout << " " << statement[i];
    std::cout << " | ";
    for(int j = 0; j < pattern.size(); j++)
        std::cout << " " << pattern[j];
    std::cout << std::endl;*/


    if(statement.size() >= pattern.size())
    {
        if(pattern.size() == 0)
        {
            if(statement.size() == 0)
                return true;
            else
                return false;
        }
        else
        {
            if(pattern[0] != "#")   // The statement does not begin with #
            {
                return match_tokens(statement[0], pattern[0], args)
                    && match_token_lists(slice(statement, 1), slice(pattern, 1), args);
            }
            else    // The statement begins with #
            {
                if(pattern.size() == 1)
                {
                    args->push_front(statement);
                    return true;
                }
                else
                {
                    // Slice the remaining tokens, moving closer and closer to the
                    // end until the string matches. If it never matches, the
                    // statement doesn't match the pattern.
                    for(int i = 1; i < statement.size(); i++)
                    {
                        //std::cout << "###" << i << "###\n";
                        if(match_token_lists(slice(statement, i), slice(pattern, 1), args))
                        {
                            args->push_front(token_list(statement.begin(), statement.begin() + i));
                            return true;
                        }
                    }
                    return false;
                }
            }

        }
    }
    else    // Pattern is longer than statement
    {
        return false;
    }
}

bool matches(token_list statement, std::string pattern, arg_list& args)
{
    args = {};
    return match_token_lists(statement, tokenize(pattern, ' '), &args);
}

std::vector<cmd_ptr> Parser::parse(std::string statement, GameState* g)
{
    statement = to_lower(statement);
    token_list tokens = tokenize(statement, ' ');
    remove_tokens(&tokens, tokens_to_remove);

    std::vector<cmd_ptr> commands = {};
    arg_list args = {};
    bool found_pattern = false;
    bool found_object = false;

    //=== Entering an object
    if(matches(tokens, "go in #", args) || matches(tokens, "enter #", args))
    {
        // Go into a portal object

        Object* o = get_object(args[0], g);
        if(o)
        {
            ComponentPortal* c_port = (ComponentPortal*)o->get_component(Component::PORTAL);
            if(c_port)
            {
                commands.push_back(std::make_shared<CmdGo>(c_port->destination));
            }
        }
    }

    //=== Going in a direction
    else if(matches(tokens, "go #", args))
    {
        DirectionId desired_direction = DIRECTION_MAX;
        if(args[0][0] == "north" || args[0][0] == "n")
            desired_direction = NORTH;
        else if(args[0][0] == "east" || args[0][0] == "e")
            desired_direction = EAST;
        else if(args[0][0] == "south" || args[0][0] == "s")
            desired_direction = SOUTH;
        else if(args[0][0] == "west" || args[0][0] == "w")
            desired_direction = WEST;
        else if(args[0][0] == "up" || args[0][0] == "u")
            desired_direction = UP;
        else if(args[0][0] == "down" || args[0][0] == "d")
            desired_direction = DOWN;

        if(desired_direction == DIRECTION_MAX)
            commands.push_back(std::make_shared<CmdDisp>("I don't know where " + join(args[0], ' ') + " is, m8."));
        else if(Object* room = g->world->get_current_room())
        {
            ComponentRoom* room_component = (ComponentRoom*)room->get_component(Component::ROOM);
            if(room_component && room_component->directions[desired_direction] != "")
            {
                commands.push_back(std::make_shared<CmdGo>(room_component->directions[desired_direction]));
            }
            else
                commands.push_back(std::make_shared<CmdDisp>("You can't go " + args[0][0] + " from here, baka!"));
        }
    }

    //=== Looking around
    else if(matches(tokens, "look around", args) || matches(tokens, "look", args) || matches(tokens, "l", args))
    {
        std::shared_ptr<CmdLookAround> look_around = std::make_shared<CmdLookAround>();
        commands.push_back(look_around);
    }

    //=== Examining something
    else if(matches(tokens, "examine #", args) || matches(tokens, "look at #", args) || matches(tokens, "look #", args))
    {
        Object* obj = get_object(args[0], g);
        if(obj)
        {
            std::shared_ptr<CmdExamine> examine = std::make_shared<CmdExamine>();
            examine->add_object(obj);
            commands.push_back(examine);
        }
        else
        {
            commands.push_back(std::make_shared<CmdDisp>("There is no " + join(args[0], ' ') + " for you to examine here."));
        }
    }

    //=== Quitting
    else if(matches(tokens, "quit", args) || matches(tokens, "q", args) || matches(tokens, "exit", args) || matches(tokens, "bye bye", args))
    {
        GameStateMenu* menu = new GameStateMenu(
                        g->engine,
                        "Are you sure you want to quit? (y/n)",
                        {{"y", {std::make_shared<CmdQuit>()}},
                         {"n", {}}});
        commands.push_back(std::make_shared<CmdAddGameState>(menu));
    }

    //=== Taking
    else if(matches(tokens, "take #", args)
            || matches(tokens, "get #", args)
            || matches(tokens, "pick up #", args)
            || matches(tokens, "pick # up", args)
            || matches(tokens, "grab #", args)
            || matches(tokens, "snatch #", args)
            || matches(tokens, "obtain #", args)
            || matches(tokens, "grasp #", args))
    {
        Object* obj = get_object(args[0], g);
        if(!obj)
            commands.push_back(std::make_shared<CmdDisp>("There's no " + join(args[0], ' ') + " for you to take here."));
        else
        {
            if(obj->has_component(Component::TAKEABLE))
            {
                commands.push_back(std::make_shared<CmdDisp>("You take the " + obj->pretty_name + "."));
                cmd_ptr cmd_take = std::make_shared<CmdTake>();
                cmd_take->add_object(obj);
                commands.push_back(cmd_take);
            }
            else
            {
                commands.push_back(std::make_shared<CmdDisp>("u huff & puff, but u can't pick up the " + obj->pretty_name + "."));
            }
        }
    }

    //=== Obscenity
    else if(matches(tokens, "fuck", args)
            || matches(tokens, "shit", args)
            || matches(tokens, "damn", args)
            || matches(tokens, "fug", args))
    {
        commands.push_back(std::make_shared<CmdDisp>("-keep it clean, nigga"));
    }

    //=== Reading
    else if(matches(tokens, "read #", args)
        || matches(tokens, "peruse #", args)
        || matches(tokens, "browse #", args))
    {
        Object* obj = get_object(args[0], g);
        if(obj)
        {
            std::shared_ptr<CmdRead> read = std::make_shared<CmdRead>();
            read->add_object(obj);
            commands.push_back(read);
        }
        else
        {
            commands.push_back(std::make_shared<CmdDisp>("There's no " + join(args[0], ' ') + "for you to read."));
        }
    }

    //=== Hitting
    else if(matches(tokens, "hit #", args)
            || matches(tokens, "flip #", args)
            || matches(tokens, "toggle #", args)
            || matches(tokens, "pull #", args)
            || matches(tokens, "push #", args)
            || matches(tokens, "yank #", args)
            || matches(tokens, "depress #", args))
    {
        Object* obj = get_object(args[0], g);
        if(obj)
        {
            ComponentHittable* c_hittable = (ComponentHittable*)obj->get_component(Component::HITTABLE);
            if(c_hittable)
            {
                commands.push_back(std::make_shared<CmdDisp>("You hit the " + join(args[0], ' ') + "."));
                std::shared_ptr<CmdHit> hit = std::make_shared<CmdHit>();
                hit->add_object(obj);
                commands.push_back(hit);
            }
            else
            {
                commands.push_back(std::make_shared<CmdDisp>("You can't hit the " + join(args[0], ' ') + ", baka!"));
            }
        }
        else
        {
            commands.push_back(std::make_shared<CmdDisp>("There's no " + join(args[0], ' ') + " for you to hit here."));
        }
    }

    //=== Wearing
    else if(matches(tokens, "wear #", args)
            || matches(tokens, "put on #", args)
            || matches(tokens, "put # on", args)
            || matches(tokens, "don #", args)
            || matches(tokens, "dress in #", args))
    {

        Object* obj = get_object(args[0], g);
        if(obj)
        {
            if(obj->has_component(Component::WEARABLE))
            {
                commands.push_back(std::make_shared<CmdDisp>("You put on the " + obj->pretty_name + "."));
                std::shared_ptr<CmdWear> wear = std::make_shared<CmdWear>();
                wear->add_object(obj);
                commands.push_back(wear);
            }
            else
            {
                commands.push_back(std::make_shared<CmdDisp>("You can't wear a " + obj->pretty_name + ", silly~"));
            }
        }
        else
        {
            commands.push_back(std::make_shared<CmdDisp>("You see no such thing."));
        }
    }

    //=== Talking to
    else if(matches(tokens, "talk to #", args)
            || matches(tokens, "talk with #", args)
            || matches(tokens, "converse with #", args))
    {

        Object* obj = get_object(args[0], g);
        if(obj)
        {
            if(obj->has_component(Component::TALKABLE))
            {
                ComponentTalkable* ctalk = (ComponentTalkable*)obj->get_component(Component::TALKABLE);
                for(int i = 0; i < ctalk->talkable_data.size(); i++)
                {
                    commands.push_back(std::make_shared<CmdDisp>(ctalk->talkable_data[i]));
                    commands.push_back(std::make_shared<CmdPause>());
                }
            }
            else
            {
                commands.push_back(std::make_shared<CmdDisp>("You can't talk to the " + obj->pretty_name + ", honey."));
            }
        }
        else
        {
            commands.push_back(std::make_shared<CmdDisp>("You can't find a " + join(args[0], ' ') + " to talk to."));
        }
    }

    //=== Climbing
    else if(matches(tokens, "climb up #", args)
            || matches(tokens, "climb #", args)
            || matches(tokens, "get on #", args))
    {
        Object* obj = get_object(args[0], g);
        if(obj)
        {
            if(obj->has_component(Component::CLIMBABLE))
            {
                ComponentClimbable* cclimb = (ComponentClimbable*)obj->get_component(Component::CLIMBABLE);
                DirectionId desired_direction = UP;
                //if(matched_pattern == 0 || matched_pattern == 3)
                    desired_direction = UP;
                //else if(matched_pattern == 1)
                //    desired_direction = DOWN;

                if(cclimb->directions[desired_direction] != "")
                {
                    commands.push_back(std::make_shared<CmdDisp>("With concerted effort and a sharp puff of breath, you climb " + dir[desired_direction].name + " the " + obj->pretty_name + "."));
                    commands.push_back(std::make_shared<CmdGo>(cclimb->directions[desired_direction]));
                }
                else
                {
                    commands.push_back(std::make_shared<CmdDisp>("The " + obj->pretty_name + " can be climbed, but cannot be climbed " + dir[desired_direction].name + "."));
                }
            }
            else
            {
                commands.push_back(std::make_shared<CmdDisp>("The " + obj->pretty_name + " cannot be climbed."));
            }
        }
        else
        {
            commands.push_back(std::make_shared<CmdDisp>("You can find no " + join(args[0], ' ') + " to climb here."));
        }
    }

    //=== Unrecognized pattern
    else
    {
        int index = -1;
        Object* obj = get_object(tokens, g, &index);
        if(obj)
        {
            token_list lhs = slice(tokens, 0, index);
            token_list rhs = slice(tokens, index + 1);
            std::string output = "";
            if(lhs.size() == 0)
                output = "I don't understand what you want to do to the " + obj->pretty_name + ".";
            else
                output = "I don't understand how to " + join(lhs, ' ') + " something" + (rhs.size() > 0 ? " " + join(rhs, ' ') : "") + ".";
            commands.push_back(std::make_shared<CmdDisp>(output));
        }
        else
        {
            commands.push_back(std::make_shared<CmdDisp>("I don't understand what u talkin bout."));
        }
    }

    return commands;
}
