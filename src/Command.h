#pragma once

class Object;
class GameState;

#include "Config.h"
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
        QUIT,
        GO,
        LOOK_AROUND,
        EXAMINE,
        SAVE,
        RESTORE,
        RESTART,
        CLIMB,
        HELLO,
        OBSCENITY,
        INVENTORY,
        TAKE,
        THROW,
        OPEN,
        CLOSE,
        READ,
        DROP,
        PUT,
        MOVE,
        EAT,
        GIVE,
        SHOUT,
        BREAK,
        KILL,
        BLESSUP,
        DRINK,
        SMELL,
        TALK_TO,
        TIE_TO,
        WEAR,
        HIT,
    };

    CommandType type;

    Command(CommandType type_in);
    virtual ~Command();

    // Runs the command.
    virtual void run(GameState* g);
};

class CmdGo : public Command
{
    public:
        std::string new_room;
        CmdGo(std::string new_room_in);
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
        Object* object;
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
        Object* object;
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

void recursive_show(GameState* g, Object* o, bool show_children, bool appearance, bool description);

class CmdLookAround : public Command
{
    public:
        CmdLookAround();
        void run(GameState* g);
};

class CmdExamine : public Command
{
    public:
        Object* object;
        CmdExamine();
        void run(GameState* g);
};

class CmdTake : public Command
{
    public:
        Object* object;
        CmdTake();
        void run(GameState* g);
};

class CmdWear : public Command
{
    public:
        Object* object;
        CmdWear();
        void run(GameState* g);
};

class CmdMove : public Command
{
    public:
        Object* object;
        CmdMove();
        void run(GameState* g);
};

class CmdEat : public Command
{
    public:
        Object* food;

        CmdEat(Object* food_in);
        void run(GameState* g);
};

class CmdGive : public Command
{
    public:
        Object* obj;
        Object* actor;

        CmdGive(Object* obj_in, Object* actor_in);
        void run(GameState* g);
};

class CmdOpen : public Command
{
    public:
        Object* obj;

        CmdOpen(Object* obj_in);
        void run(GameState* g);
};

class CmdClose : public Command
{
    public:
        Object* obj;

        CmdClose(Object* obj_in);
        void run(GameState* g);
};

class CmdTieTo : public Command
{
    public:
        Object* tie;
        Object* tie_to;

        CmdTieTo(Object* tie_in, Object* tie_to_in);
        void run(GameState* g);
};

class CmdInv : public Command
{
    public:
        Object* player;

        CmdInv(Object* player_in);
        void run(GameState* g);
};

class CmdThrow : public Command
{
    public:
        Object* projectile;
        Object* target;

        CmdThrow(Object* projectile_in, Object* target_in);
        void run(GameState* g);
};
