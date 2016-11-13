#pragma once

class GameState;
class Terminal;
class Object;

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
        ADD_GAMESTATE,
        CUSTOM
    };

    CommandType type;
    std::vector<Object*> objects;

    Command(CommandType type_in);
    virtual ~Command();

    virtual bool parse(std::vector<std::string> tokens);

    virtual void run_with_callbacks(GameState* g);
    virtual void run(GameState* g);
    virtual void add_object(Object* o);
};

typedef std::shared_ptr<Command> cmd_ptr;

class CmdDisp : public Command
{
    public:
        std::string str;
        bool append_newline;
        Terminal* terminal;

        CmdDisp(std::string str_in, bool append_newline_in = true);
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
        std::string new_room;
        CmdSetRoom(std::string new_room_in);
        void run(GameState* g);
};

class CmdDescribe : public Command
{
    public:
        bool describe_this;
        bool deep;
        CmdDescribe(bool describe_this_in = true, bool deep_in = true);
        bool parse(std::vector<std::string> tokens);
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

class CmdCustom : public Command
{
    public:
        std::function<void(GameState*)> fn;
        CmdCustom(std::function<void(GameState*)> fn_in);
        void run(GameState* g);
};
