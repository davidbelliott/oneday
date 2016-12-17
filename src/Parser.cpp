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
        if(found_object &&
                (!found_object->active ||
                 !(found_object->discovered || found_object->parent == g->world->get_player())))
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
    std::string lower_str = "";
    for(int i = 0; i < str.size(); i++)
        lower_str.push_back(std::tolower(str[i]));
    return lower_str;
}

std::string to_upper(std::string str)
{
    std::string upper_str = "";
    for(int i = 0; i < str.size(); i++)
        upper_str.push_back(std::toupper(str[i]));
    return upper_str;
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

cmd_ptr Parser::parse(std::string statement, GameState* g)
{
    std::vector<cmd_ptr> commands = {};
    std::vector<std::string> errors = {};

    statement = to_lower(statement);
    token_list tokens = tokenize(statement, ' ');
    remove_tokens(&tokens, tokens_to_remove);

    arg_list args = {};

    //=== Going
    if(matches(tokens, "go in #", args) || matches(tokens, "enter #", args))
    {
        Object* o = get_object(args[0], g);
        if(o)
        {
            ComponentPortal* c_port = (ComponentPortal*)o->get_component(Component::PORTAL);
            if(c_port)
                commands.push_back(std::make_shared<CmdGo>(c_port->destination));
            else
                errors.push_back("You can't go into the " + o->pretty_name + ".");
        }
        else
            errors.push_back("You can find no " + join(args[0], ' ') + " to go in.");
    }
    else if(matches(tokens, "go north", args)
            || matches(tokens, "north", args)
            || matches(tokens, "n", args))
    {
        try_to_go(NORTH, g, &commands, &errors);
    }
    else if(matches(tokens, "go east", args)
            || matches(tokens, "east", args)
            || matches(tokens, "e", args))
    {
        try_to_go(EAST, g, &commands, &errors);
    }
    else if(matches(tokens, "go south", args)
            || matches(tokens, "south", args)
            || matches(tokens, "s", args))
    {
        try_to_go(SOUTH, g, &commands, &errors);
    }
    else if(matches(tokens, "go west", args)
            || matches(tokens, "west", args)
            || matches(tokens, "w", args))
    {
        try_to_go(WEST, g, &commands, &errors);
    }
    else if(matches(tokens, "go up", args)
            || matches(tokens, "climb up", args)
            || matches(tokens, "climb", args)
            || matches(tokens, "climb #", args)
            || matches(tokens, "up", args)
            || matches(tokens, "u", args))
    {
        try_to_go(UP, g, &commands, &errors);
    }
    else if(matches(tokens, "go down", args)
            || matches(tokens, "climb down", args)
            || matches(tokens, "climb down #", args)
            || matches(tokens, "down", args)
            || matches(tokens, "d", args))
    {
        try_to_go(DOWN, g, &commands, &errors);
    }
    else if(matches(tokens, "go #", args))
    {
        errors.push_back(join(args[0], ' ') + " isn't a valid direction.");
    }

    //=== Looking around
    if(matches(tokens, "look around", args)
            || matches(tokens, "look", args)
            || matches(tokens, "l", args)
            || matches(tokens, "x", args))
    {
        commands.push_back(std::make_shared<CmdLookAround>());
    }

    //=== Examining something
    if(matches(tokens, "examine #", args)
        || matches(tokens, "look at #", args)
        || matches(tokens, "look #", args)
        || matches(tokens, "l #", args)
        || matches(tokens, "x #", args))
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
            errors.push_back("There is no " + join(args[0], ' ') + " for you to examine here.");
        }
    }

    //=== Quitting
    if(matches(tokens, "quit", args)
        || matches(tokens, "q", args)
        || matches(tokens, "exit", args)
        || matches(tokens, "bye bye", args))
    {
        GameStateMenu* menu = new GameStateMenu(
                        g->engine,
                        "Are you sure you want to quit? (y/n)",
                        {{"y", {std::make_shared<CmdQuit>()}},
                         {"n", {}}});
        commands.push_back(std::make_shared<CmdAddGameState>(menu));
    }

    //=== Taking
    if(matches(tokens, "take #", args)
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
            errors.push_back("There's no " + join(args[0], ' ') + " for you to take here.");
        else
        {
            if(obj->has_component(Component::TAKEABLE))
            {
                cmd_ptr cmd_take = std::make_shared<CmdTake>();
                cmd_take->add_object(obj);
                commands.push_back(cmd_take);
            }
            else
            {
                errors.push_back("Why would you want to pick up a " + obj->pretty_name + "?");
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
            errors.push_back("There's no " + join(args[0], ' ') + "for you to read.");
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
                std::shared_ptr<CmdHit> hit = std::make_shared<CmdHit>();
                hit->add_object(obj);
                commands.push_back(hit);
            }
            else
                errors.push_back("You can't hit the " + join(args[0], ' ') + ", baka!");
        }
        else
            errors.push_back("There's no " + join(args[0], ' ') + " for you to hit here.");
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
                std::shared_ptr<CmdWear> wear = std::make_shared<CmdWear>();
                wear->add_object(obj);
                commands.push_back(wear);
            }
            else
                errors.push_back("You can't wear a " + obj->pretty_name + ", silly~");
        }
        else
            errors.push_back("You see no such thing.");
    }

    //=== Talking to
    else if(matches(tokens, "talk to #", args)
            || matches(tokens, "talk with #", args)
            || matches(tokens, "converse with #", args))
    {

        Object* obj = get_object(args[0], g);
        if(obj)
        {
            auto talk_to = std::make_shared<CmdTalkTo>();
            talk_to->add_object(obj);
            commands.push_back(talk_to);
        }
        else
            errors.push_back("You can't find a " + join(args[0], ' ') + " to talk to.");
    }

    //=== Moving an object
    if(matches(tokens, "move #", args))
    {
        Object* obj = get_object(args[0], g);
        if(obj)
        {
            auto move = std::make_shared<CmdMove>();
            move->add_object(obj);
            commands.push_back(move);
        }
        else
            errors.push_back("You can find no " + join(args[0], ' ') + " to move here.");
    }

    //=== Eating an object
    if(matches(tokens, "eat #", args))
    {
        Object* obj = get_object(args[0], g);
        if(obj)
        {
            if(obj->has_component(Component::EDIBLE))
                commands.push_back(std::make_shared<CmdEat>(obj));
            else
                errors.push_back("The " + obj->pretty_name + " is not edible.");
        }
        else
            errors.push_back("There is no " + join(args[0], ' ') + " here.");
    }
            
    //=== Feeding an object to an actor
    if(matches(tokens, "feed # to #", args))
    {
        Object* food = get_object(args[0], g);
        Object* actor = get_object(args[1], g);
        if(actor)
        {
            if(actor->has_component(Component::TALKABLE))
            {
                if(food)
                {
                    if(food->has_component(Component::EDIBLE))
                        commands.push_back(std::make_shared<CmdFeed>(food, actor));
                    else
                        errors.push_back("The " + food->pretty_name + " isn't edible.");
                }
                else
                    errors.push_back("There is no " + join(args[1], ' ') + " here.");
            }
            else
                errors.push_back(">He thinks he can feed something to a " + actor->pretty_name);
        }
        else
            errors.push_back("There is no " + join(args[0], ' ') + "here.");
    }

    //=== Opening something
    if(matches(tokens, "open #", args))
    {
        Object* obj = get_object(args[0], g);
        if(obj)
        {
            ComponentOpenClose* c_open = (ComponentOpenClose*)obj->get_component(Component::OPEN_CLOSE);
            if(c_open)
            {
                if(!c_open->open)
                    commands.push_back(std::make_shared<CmdOpen>(obj));
                else
                    errors.push_back("It's already open.");
            }
            else
                errors.push_back("It can't be opened.");
        }
        else
            errors.push_back("You see no such thing.");
    }

    //=== Yelling
    if(matches(tokens, "yell", args)
            || matches(tokens, "shout", args)
            || matches(tokens, "holler", args)
            || matches(tokens, "scream", args))
    {
        commands.push_back(std::make_shared<CmdDisp>("(Jamal) SHEEEIT!"));
    }
    else if(matches(tokens, "yell #", args)
            || matches(tokens, "shout #", args)
            || matches(tokens, "holler #", args)
            || matches(tokens, "scream #", args))
    {
        commands.push_back(std::make_shared<CmdDisp>("(Jamal) " + to_upper(join(args[0], ' ')) + "!"));
    }

    cmd_ptr command = nullptr;

    if(commands.size() == 1)
        command = commands[0];
    else if(commands.size() > 1)
        command =  std::make_shared<CmdDisp>("Your statement was ambiguous.");
    else if(errors.size() == 1)
        command =  std::make_shared<CmdDisp>(errors[0]);
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
                command = std::make_shared<CmdDisp>("I don't understand what you want to do to the " + obj->pretty_name + ".");
            else
                command = std::make_shared<CmdDisp>("I don't understand how to " + join(lhs, ' ') + " something.");
        }
        else
            command = std::make_shared<CmdDisp>("-wat u talkin bout boi");
    }
    return command;
}

void try_to_go(DirectionId direction, GameState* g, std::vector<cmd_ptr>* commands, std::vector<std::string>* errors)
{
    bool can_go = true;
    if(Object* room = g->world->get_current_room())
    {
        ComponentRoom* room_component = (ComponentRoom*)room->get_component(Component::ROOM);
        if(room_component && room_component->directions[direction] != "")
            commands->push_back(std::make_shared<CmdGo>(room_component->directions[direction]));
        else
            can_go = false;
    }
    else
        can_go = false;
    if(!can_go)
        errors->push_back("You can't go " + dir[direction].name + " from here, baka!");
}
