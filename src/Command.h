#pragma once

class GameState;
class GameStateTerminal;
class Receiver;
class Terminal;
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Command
{
public:
    enum CommandType
    {
        DISP,
        OUTPUT,
        CLEAR,
        SETCOLOR,
        INPUT,
        PAUSE,
        UNPAUSE,
        CHANGE_ROOM,
        SET_OBJECTIVE,
        ADD_GAMESTATE
    };

    CommandType type;

    Command(CommandType type_in)
    : type(type_in)
    {
    }

    virtual ~Command()
    {
    }

    virtual void run(Receiver* r)
    {
    }
};

typedef std::shared_ptr<Command> cmd_ptr;

class CmdDisp : public Command
{
    public:
        std::string str;
        Terminal* terminal;

        CmdDisp(std::string str_in)
        :   Command(DISP),
            str(str_in)
        {
        }

        void run(Receiver* r)
        {
            //terminal->disp(str);
        }
};

class CmdOutput : public Command
{
    public:
        int x;
        int y;
        std::string str;

        CmdOutput(int x_in, int y_in, std::string str_in)
            : Event(CMD_OUTPUT),
            x(x_in),
            y(y_in),
            str(str_in)
        {
        }

        void run(Receiver* r)
        {
            //terminal->output(x, y, str);
        }
};

class CmdClear : public Command
{
    public:
        CmdClear()
            : Event(CMD_CLEAR)
        {}

        void run(Receiver* r)
        {

        }
};

class CmdSetColor : public Command
{
    public:
        sf::Color color;
        CmdSetColor(sf::Color color_in = config::colors[config::color_default_fg])
            : Event(CMD_SETCOLOR),
              color(color_in)
        {}

        void run(Receiver* r)
        {

        }
};

class CmdInput : public Command
{
    public:
        CmdInput()
            : Event(CMD_INPUT)
        {}

        void run(Receiver* r)
        {

        }
};

class CmdPause : public Command
{
    public:
        CmdPause()
            : Event(CMD_PAUSE)
        {}

        void run(Receiver* r)
        {

        }
};

class CmdUnpause : public Command
{
    public:
        CmdUnpause()
            : Event(CMD_UNPAUSE)
        {}

        void run(Receiver* r)
        {

        }
};

class CmdSetObjective : public Command
{
    public:
        std::string objective;
        CmdSetObjective(std::string objective_in)
            : Event(CMD_SET_OBJECTIVE),
              objective(objective_in)
        {}

        void run(Receiver* r)
        {

        }
};

class CmdAddGameState : public Command
{
    public:
        GameState* state_to_add;
        CmdAddGameState(GameState* state_to_add_in)
            : Event(CMD_ADD_GAMESTATE),
              state_to_add(state_to_add_in)
        {}

        void run(Receiver* r)
        {

        }
};
