#include "Parser.h"
#include "World.h"
#include "Player.h"
#include "GameState.h"
#include "Engine.h"
#include "Terminal.h"
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

    parsing_cmds = {
        new CmdGo(),
        new CmdQuit(),
        new CmdHit(),
        //new CmdShout(),
        new CmdRead(),
        new CmdTalkTo(),
        //new CmdHelp(),
        new CmdLookAround(),
        new CmdExamine(),
        new CmdTake(),
        new CmdWear(),
        new CmdMove(),
        new CmdEat(),
        new CmdGive(),
        new CmdOpen(),
        new CmdClose(),
        new CmdTieTo(),
        new CmdInv(),
        new CmdThrow()};
}

Parser::~Parser()
{
    for(int i = 0; i < parsing_cmds.size(); i++)
        delete parsing_cmds[i];
}

Object* get_object(token_list tokens, World* world, int* index)
{
    Object* found_object = nullptr;
    int i = 0;
    for( ; i < tokens.size() && !found_object; i++)
    {
        found_object = world->get_current_room()->get_indirect_child(tokens[i], 0);
        if(found_object &&
                (!found_object->active ||
                 !(found_object->discovered || found_object->parent == world->get_player())))
            found_object = nullptr;
        if(!found_object)
        {
            found_object = world->get_player()->get_indirect_child(tokens[i], 0);
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

bool matches(std::string statement, std::string pattern, arg_list& args)
{
    args = {};
    return match_token_lists(tokenize(statement, ' '), tokenize(pattern, ' '), &args);
}

Command* Parser::parse(std::string statement, GameState* g)
{
    std::vector<Command*> commands;
    std::vector<std::string> errors;

    for(int i = 0; i < parsing_cmds.size(); i++)
    {
        bool match = parsing_cmds[i]->match(statement, &errors, g->world);
        if(match)
            commands.push_back(parsing_cmds[i]);
    }

    Command* command = nullptr;

    if(commands.size() == 1)
        command = commands[0];
    else if(commands.size() > 1)
        g->engine->terminal->disp("Your statement was ambiguous.");
    else if(errors.size() >= 1)
        g->engine->terminal->disp(errors[0]);
    else
        g->engine->terminal->disp("-wat u talkin bout boi");

        //command =  new CmdDisp(errors[0]);
    /*else
    {
        int index = -1;
        Object* obj = get_object(tokens, g, &index);
        if(obj)
        {
            token_list lhs = slice(tokens, 0, index);
            token_list rhs = slice(tokens, index + 1);
            std::string output = "";
            if(lhs.size() == 0)
                //command = new CmdDisp("I don't understand what you want to do to the " + obj->pretty_name + ".");
            else
                //command = new CmdDisp("I don't understand how to " + join(lhs, ' ') + " something.");
        }
        //else
            //command = new CmdDisp("-wat u talkin bout boi");
    }*/
    return command;


    //statement = to_lower(statement);
    //token_list tokens = tokenize(statement, ' ');
    //remove_tokens(&tokens, tokens_to_remove);

    //arg_list args = {};
/*
    //=== Going

    //=== Looking around

    //=== Examining something

    //=== Quitting

    //=== Taking

    //=== Obscenity
    else if(matches(tokens, "fuck", args)
            || matches(tokens, "shit", args)
            || matches(tokens, "damn", args)
            || matches(tokens, "fug", args))
    {
        errors.push_back("-keep it clean, nigga");
    }

    //=== Reading

    //=== Hitting

    //=== Wearing

    //=== Talking to

    //=== Moving an object

    //=== Eating an object
            
    //=== Giving an object to an actor

    //=== Opening something

    //=== Closing something

    //=== Yelling
    if(matches(tokens, "yell", args)
            || matches(tokens, "shout", args)
            || matches(tokens, "holler", args)
            || matches(tokens, "scream", args))
    {
        //commands.push_back(new CmdDisp("(Jamal) SHEEEIT!"));
    }
    else if(matches(tokens, "yell #", args)
            || matches(tokens, "shout #", args)
            || matches(tokens, "holler #", args)
            || matches(tokens, "scream #", args))
    {
        //commands.push_back(new CmdDisp("(Jamal) " + to_upper(join(args[0], ' ')) + "!"));
    }

    //=== Tying


    //=== Inventory

    //=== Throwing


*/

}

