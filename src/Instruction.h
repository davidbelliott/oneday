#pragma once
#include "Command.h"
#include "Token.h"

// Encapsulates a text-based user instruction to the gamestate, e.g. "go north"
// Created by the parser from user input
// Can be translated into zero or more commands by calling compile(GameState* g)
class Instruction
{
    public:
        enum Type
        {
            GO,
            LOOK,
            QUIT,
            TAKE,
            WEAR,
            TOGGLE,
            READ,
            SHOUT,
            TALK_TO,
            HELP,
            OBSCENITY,
            CLIMB,
            INSTRUCTION_MAX
        } type;
        std::vector<std::string> patterns;
        int matched_pattern;
        arg_list args;

        Instruction(Type type_in, int matched_pattern_in, arg_list args_in);

        virtual ~Instruction();

        // Compiles the instruction into zero or more commands
        virtual std::vector<cmd_ptr> compile(GameState* g);
};

class InstructionGo : public Instruction
{
    public:
        InstructionGo(int matched_pattern_in, arg_list args_in);
        std::vector<cmd_ptr> compile(GameState* g);
};

class InstructionLook : public Instruction
{
    public:
        InstructionLook(int matched_pattern_in, arg_list args_in);
        std::vector<cmd_ptr> compile(GameState* g);
};

class InstructionQuit : public Instruction
{
    public:
        InstructionQuit(int matched_pattern_in, arg_list args_in);
        std::vector<cmd_ptr> compile(GameState* g);
};

class InstructionTake : public Instruction
{
    public:
        InstructionTake(int matched_pattern_in, arg_list args_in);
        std::vector<cmd_ptr> compile(GameState* g);
};

class InstructionObscenity : public Instruction
{
    public:
        InstructionObscenity(int matched_pattern_in, arg_list args_in);
        std::vector<cmd_ptr> compile(GameState* g);
};

class InstructionRead : public Instruction
{
    public:
        InstructionRead(int matched_pattern_in, arg_list args_in);
        std::vector<cmd_ptr> compile(GameState* g);
};

class InstructionToggle : public Instruction
{
    public:
        InstructionToggle(int matched_pattern_in, arg_list args_in);
        std::vector<cmd_ptr> compile(GameState* g);
};

class InstructionWear : public Instruction
{
    public:
        InstructionWear(int matched_pattern_in, arg_list args_in);
        std::vector<cmd_ptr> compile(GameState* g);
};

class InstructionTalkTo : public Instruction
{
    public:
        InstructionTalkTo(int matched_pattern_in, arg_list args_in);
        std::vector<cmd_ptr> compile(GameState* g);
};

class InstructionClimb : public Instruction
{
    public:
        InstructionClimb(int matched_pattern_in, arg_list args_in);
        std::vector<cmd_ptr> compile(GameState* g);
};

/*class InstructionHit : public Instruction
{
    public:
        InstructionHit(int matched_pattern_in, arg_list args_in);
        std::vector<cmd_ptr> compile(GameState* g);
};

class InstructionShout : public Instruction
{
    public:
        InstructionShout(int matched_pattern_in, arg_list args_in);
        std::vector<cmd_ptr> compile(GameState* g);
};

class InstructionHelp : public Instruction
{
    public:
        InstructionHelp(int matched_pattern_in, arg_list args_in);
        std::vector<cmd_ptr> compile(GameState* g);
};*/

typedef std::shared_ptr<Instruction> InstructionPtr;

InstructionPtr make_instruction(Instruction::Type type, int matched_pattern_in, arg_list args);

// Iterates through the provided tokens until it is able to find one corresponding
// to an object in the gamestate provided.
Object* get_object(token_list tokens, GameState* g);
