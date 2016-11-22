#pragma once

class Object;
class GameState;

#include "Config.h"
#include <SFML/Graphics.hpp>
#include <memory>

// Commands are the functional backbone of the engine.
// They are generated within gamestates in response to events, and the
// gamestates store them on queues before running them.
// A command can operate on zero or more objects.
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
        ADD_GAMESTATE,
        ADD_OBJECT,
        RM_OBJECT,
        INV_ADD_OBJECT,
        INV_RM_OBJECT,
        CLOTHES_ADD_OBJECT,
        CLOTHES_REMOVE_OBJECT,
        DESCRIBE,
        CUSTOM
    };

    CommandType type;
    std::vector<Object*> objects;
    std::vector<std::string> patterns;

    Command(CommandType type_in);
    virtual ~Command();

    // Adds an object to the list of objects that will be acted on
    virtual void add_object(Object* o);

    // === Methods subclasses should overload ===

    // Runs the command.
    virtual void run(GameState* g);
};

typedef std::shared_ptr<Command> cmd_ptr;

class CmdDisp : public Command
{
    public:
        std::string str;
        bool append_newline;

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

class CmdQuit : public Command
{
    public:
        CmdQuit();
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

class CmdDescribe : public Command
{
    public:
        CmdDescribe();
        void describe(GameState* g, Object* o);
        void run(GameState* g);
};
