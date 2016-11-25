#include "Parser.h"
#include "World.h"
#include "Player.h"
#include "GameState.h"
#include "Command.h"
#include <iostream>

Parser::Parser()
 : instruction_lookup_table({}),
   tokens_to_remove({})
{
    for(int i = 0; i < static_cast<int>(Instruction::INSTRUCTION_MAX); i++)
    {
        InstructionPtr instruction = make_instruction(static_cast<Instruction::Type>(i), 0, {});
        if(instruction)
            instruction_lookup_table.push_back(instruction);
    }

    tokens_to_remove = {
        "the",
        "a",
        "and"
    };
}

Parser::~Parser()
{
}

std::string to_lower(std::string str)
{
    std::string lower_str;
    for(int i = 0; i < str.size(); i++)
        lower_str.push_back(std::tolower(str[i]));
    return lower_str;
}

token_list slice(token_list list, int start)
{
    token_list slice_list(list.begin() + start, list.end());
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

InstructionPtr Parser::parse(std::string statement, GameState* g)
{
    statement = to_lower(statement);
    token_list tokens = tokenize(statement, ' ');
    remove_tokens(&tokens, tokens_to_remove);

    InstructionPtr instruction = nullptr;
    for(int i = 0; i < instruction_lookup_table.size() && !instruction; i++)
    {
        for(int j = 0; j < instruction_lookup_table[i]->patterns.size() && !instruction; j++)
        {
            arg_list args = {};
            if(match_token_lists(tokens, tokenize(instruction_lookup_table[i]->patterns[j], ' '), &args))
            {
                instruction = make_instruction(instruction_lookup_table[i]->type, j, args);
            }
        }
    }
    return instruction;
}
