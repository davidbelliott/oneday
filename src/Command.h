#pragma once

class GameState;
class Terminal;

#include "Object.h"
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Command
{
public:
    enum CommandType
    {
        // View commands
        DISP,
        OUTPUT,
        CLEAR,
        SETCOLOR,
        // Controller commands
        INPUT,
        PAUSE,
        UNPAUSE,
        QUIT,
        // Model commands
        SET_ROOM,
        SET_OBJECTIVE,
        DESCRIBE,
        TAKE,
        WEAR,
        OPEN,
        HIT,
        READ,
        SHOUT,
        TALK_TO,
        HELP,
        ADD_GAMESTATE
    };

    CommandType type;
    std::vector<Object*> objects;

    Command(CommandType type_in);

    virtual ~Command();
    virtual void run(GameState* g);
    virtual void add_object(Object* o);
};

typedef std::shared_ptr<Command> cmd_ptr;

class CmdDisp : public Command
{
    public:
        std::string str;
        Terminal* terminal;

        CmdDisp(std::string str_in);
        void run(GameState* g);
};

class CmdOutput : public Command
{
    public:
        int x;
        int y;
        std::string str;

        CmdOutput(int x_in, int y_in, std::string str_in);
        void run(GameState* g);
};

class CmdClear : public Command
{
    public:
        CmdClear();
        void run(GameState* g);
};

class CmdSetColor : public Command
{
    public:
        sf::Color color;
        CmdSetColor(sf::Color color_in = config::colors[config::color_default_fg]);
        void run(GameState* g);
};

class CmdInput : public Command
{
    public:
        CmdInput();
        void run(GameState* g);
};

class CmdPause : public Command
{
    public:
        CmdPause();
        void run(GameState* g);
};

class CmdUnpause : public Command
{
    public:
        CmdUnpause();
        void run(GameState* g);
};

class CmdSetObjective : public Command
{
    public:
        std::string objective;
        CmdSetObjective(std::string objective_in);
        void run(GameState* g);
};

class CmdAddGameState : public Command
{
    public:
        GameState* state_to_add;
        CmdAddGameState(GameState* state_to_add_in);
        void run(GameState* g);
};

class CmdSetRoom : public Command
{
    public:
        World* world;
        std::string new_room;
        CmdSetRoom(World* world_in, std::string new_room_in);
        void run(GameState* g);
};

class CmdDescribe : public Command
{
    public:
        CmdDescribe();
        void run(GameState* g);
};

class CmdQuit : public Command
{
    public:
        CmdQuit();
        void run(GameState* g);
};

class CmdTake : public Command
{
    public:
        CmdTake();
        void run(GameState* g);
};

class CmdWear : public Command
{
    public:
        CmdWear();
        void run(GameState* g);
};

class CmdHit : public Command
{
    public:
        CmdHit();
        void run(GameState* g);
};

class CmdShout : public Command
{
    public:
        CmdShout();
        void run(GameState* g);
};

class CmdRead : public Command
{
    public:
        CmdRead();
        void run(GameState* g);
};

class CmdTalkTo : public Command
{
    public:
        CmdTalkTo();
        void run(GameState* g);
};

class CmdHelp : public Command
{
    public:
        CmdHelp();
        void run(GameState* g);
};
