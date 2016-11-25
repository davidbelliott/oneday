#include "Instruction.h"
#include "Directions.h"
#include "GameState.h"
#include "Engine.h"
#include "World.h"
#include "Parser.h"

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
        { "go to #" },
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
            ComponentRoom* room_component = (ComponentRoom*)room->get_component(Component::ROOM);
            if(desired_direction == DIRECTION_MAX)
                commands.push_back(std::make_shared<CmdDisp>("I don't know where the " + args[0][0] + " is, m8."));
            else if(room && room_component && room_component->directions[desired_direction] != "")
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

InstructionTake::InstructionTake(int matched_pattern_in, arg_list args_in)
    : Instruction(TAKE, matched_pattern_in, args_in)
{
    patterns = {
        { "take #" },
        { "get #" },
        { "pick up #" },
        { "pick # up" },
        { "grab #" },
        { "snatch #" },
        { "obtain #" },
        { "grasp #" } };
}

std::vector<cmd_ptr> InstructionTake::compile(GameState* g)
{
    std::vector<cmd_ptr> commands = {};
    Object* obj = get_object(args[0], g);
    if(!obj)
    {
        commands.push_back(std::make_shared<CmdDisp>("There's no " + join(args[0], ' ') + " for you to take here."));
    }
    else
    {
        cmd_ptr cmd_take = std::make_shared<CmdTake>();
        cmd_take->add_object(obj);
        commands.push_back(cmd_take);
        commands.push_back(std::make_shared<CmdDisp>("You take the " + obj->pretty_name + "."));
    }
    return commands;
}

InstructionObscenity::InstructionObscenity(int matched_pattern_in, arg_list args_in)
    : Instruction(OBSCENITY, matched_pattern_in, args_in)
{
    std::vector<std::string> obscenities = {
        "fuck",
        "fucking",
        "shit",
        "damn",
        "bitch",
        "nigger",
        "faggot",
        "bastard",
        "hell",
        "bloody",
        "durned",
        "fug"
    };
    for(int i = 0; i < obscenities.size(); i++)
    {
        patterns.push_back(obscenities[i]);
        patterns.push_back("# " + obscenities[i]);
        patterns.push_back(obscenities[i] + " #");
        patterns.push_back("# " + obscenities[i] + " #");
    }
}

std::vector<cmd_ptr> InstructionObscenity::compile(GameState* g)
{
    std::vector<cmd_ptr> commands = {};
    commands.push_back(std::make_shared<CmdDisp>("-keep it clean, nigga."));
    return commands;
}

InstructionRead::InstructionRead(int matched_pattern_in, arg_list args_in)
    : Instruction(READ, matched_pattern_in, args_in)
{
    patterns = {
        "read #",
        "peruse #",
        "browse #"
    };
}

std::vector<cmd_ptr> InstructionRead::compile(GameState* g)
{
    std::vector<cmd_ptr> commands = {};
    Object* obj = get_object(args[0], g);
    if(obj)
    {
        ComponentText* text = (ComponentText*)obj->get_component(Component::TEXT);
        if(text)
        {
            commands.push_back(std::make_shared<CmdDisp>("The " + obj->pretty_name + " reads:"));
            commands.push_back(std::make_shared<CmdDisp>(text->text));
        }
        else
        {
            commands.push_back(std::make_shared<CmdDisp>("There's nothing to read on the " + obj->pretty_name + "."));
        }
    }
    else
    {
        commands.push_back(std::make_shared<CmdDisp>("There's no " + join(args[0], ' ') + "for you to read."));
    }
    return commands;
}

/*
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
    else if(type == Instruction::TAKE)
        instruction = std::make_shared<InstructionTake>(matched_pattern_in, args);
    else if(type == Instruction::OBSCENITY)
        instruction = std::make_shared<InstructionObscenity>(matched_pattern_in, args);
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

