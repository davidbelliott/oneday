#include "Instruction.h"
#include "Directions.h"
#include "GameState.h"
#include "Engine.h"
#include "World.h"

Instruction::Instruction(Type type_in, int matched_pattern_in,  arg_list args_in)
    : type(type_in),
    matched_pattern(matched_pattern_in),
    args(args_in)
{ }

Instruction::~Instruction()
{ }

std::vector<cmd_ptr> Instruction::compile(GameState* g)
{
    return {};
}

InstructionGo::InstructionGo(int matched_pattern_in, arg_list args_in)
    : Instruction(GO, matched_pattern_in, args_in)
{
    patterns = {
        { "go to the #" },
        { "go #" },
        { "head #" },
        { "n" },
        { "e" },
        { "s" },
        { "w" },
        { "u" },
        { "d" } };
}

std::vector<cmd_ptr> InstructionGo::compile(GameState* g)
{
    std::vector<cmd_ptr> commands = {};
    if(matched_pattern >= 0 && matched_pattern <= 3)
    {
        DirectionId desired_direction = DIRECTION_MAX;
        if(args[0][0] == "north")
            desired_direction = NORTH;
        else if(args[0][0] == "east")
            desired_direction = EAST;
        else if(args[0][0] == "south")
            desired_direction = SOUTH;
        else if(args[0][0] == "west")
            desired_direction = WEST;

        Object* room = g->engine->world->get_current_room();
        if(room)
        {
            auto room_component = std::static_pointer_cast<ComponentRoom>(room->get_component(Component::ROOM));
            if(desired_direction == DIRECTION_MAX)
                commands.push_back(std::make_shared<CmdDisp>("I don't know where " + args[0][0] + " is, m8."));
            else if(room && room_component->directions[desired_direction] != "")
            {
                commands.push_back(std::make_shared<CmdSetRoom>(room_component->directions[desired_direction]));
                std::shared_ptr<CmdDescribe> describe = std::make_shared<CmdDescribe>();
                describe->add_object(g->engine->world->get_direct_child(room_component->directions[desired_direction], 0));
                commands.push_back(describe);
            }
            else
                commands.push_back(std::make_shared<CmdDisp>("You can't go " + args[0][0] + " from here, baka!"));
        }
    }
    return commands;
}

InstructionLook::InstructionLook(int matched_pattern_in, arg_list args_in)
    : Instruction(LOOK, matched_pattern_in, args_in)
{
    patterns = {
        { "look around" },
        { "look in #" },
        { "look at #" },
        { "look #" }
    };
}

std::vector<cmd_ptr> InstructionLook::compile(GameState* g)
{
    std::vector<cmd_ptr> commands = {};
    if(matched_pattern == 0)    // look around
    {
        cmd_ptr describe = std::make_shared<CmdDescribe>();
        describe->add_object(g->engine->world->get_current_room());
        commands.push_back(describe);
    }
    else if(matched_pattern >= 1 && matched_pattern <= 3)
    {
        Object* obj = get_object(args[0], g);
        if(obj)
        {
            cmd_ptr describe = std::make_shared<CmdDescribe>();
            describe->add_object(obj);
            commands.push_back(describe);
        }
        else
        {
            commands.push_back(std::make_shared<CmdDisp>("There is no " + args[0][0] + " for you to look at here."));
        }
    }
    return commands;
}

InstructionQuit::InstructionQuit(int matched_pattern_in, arg_list args_in)
    : Instruction(QUIT, matched_pattern_in, args_in)
{
    patterns = {
        { "quit" },
        { "exit" },
        { "bye bye" },
        { "q" }
    };
}

std::vector<cmd_ptr> InstructionQuit::compile(GameState* g)
{
    return {std::make_shared<CmdQuit>()};
}
/*
std::vector<cmd_ptr> InstructionTake::compile(GameState* g)
{
    return { };
}

std::vector<cmd_ptr> InstructionWear::compile(GameState* g)
{
    return { };
}

std::vector<cmd_ptr> InstructionHit::compile(GameState* g)
{
    return { };
}

std::vector<cmd_ptr> InstructionRead::compile(GameState* g)
{
    return { };
}

std::vector<cmd_ptr> InstructionShout::compile(GameState* g)
{
    return { };
}

std::vector<cmd_ptr> InstructionTalkTo::compile(GameState* g)
{
    return { };
}

std::vector<cmd_ptr> InstructionHelp::compile(GameState* g)
{
    return { };
}*/

InstructionPtr make_instruction(Instruction::Type type, int matched_pattern_in, arg_list args)
{
    InstructionPtr instruction = nullptr;
    if(type == Instruction::GO)
        instruction = std::make_shared<InstructionGo>(matched_pattern_in, args);
    else if(type == Instruction::LOOK)
        instruction = std::make_shared<InstructionLook>(matched_pattern_in, args);
    else if(type == Instruction::QUIT)
        instruction = std::make_shared<InstructionQuit>(matched_pattern_in, args);
    return instruction;
}

Object* get_object(token_list tokens, GameState* g)
{
    Object* found_object = nullptr;
    for(int i = 0; i < tokens.size() && !found_object; i++)
    {
        found_object = g->engine->world->get_indirect_child(tokens[i], 0);
    }
    return found_object;
}
