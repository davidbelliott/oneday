#pragma once

class Object;
class GameState;

#include "Config.h"
#include "Parser.h"
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

    // Tries to make a command of its own type matching the given input.
    // Returns true if matches, false otherwise
    virtual bool match(std::string str, std::vector<std::string>* errors, World* world) { return bool(); }

    // Runs the command.
    virtual void run(GameState* g);
};

class CmdGo : public Command
{
    private:
        bool try_to_go(DirectionId direction, std::vector<std::string>* errors, World* world);
    public:
        std::string new_room;
        CmdGo();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdQuit : public Command
{
    public:
        CmdQuit();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdHit : public Command
{
    public:
        Object* object;
        CmdHit();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdShout : public Command
{
    public:
        CmdShout();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdRead : public Command
{
    public:
        Object* object;

        CmdRead();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdTalkTo : public Command
{
    public:
        CmdTalkTo();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdHelp : public Command
{
    public:
        CmdHelp();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

void recursive_show(GameState* g, Object* o, bool show_children, bool appearance, bool description);

class CmdLookAround : public Command
{
    public:
        CmdLookAround();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdExamine : public Command
{
    public:
        Object* object;

        CmdExamine();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdTake : public Command
{
    public:
        Object* object;

        CmdTake();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdWear : public Command
{
    public:
        Object* object;

        CmdWear();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdMove : public Command
{
    public:
        Object* object;

        CmdMove();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdEat : public Command
{
    public:
        Object* food;

        CmdEat();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdGive : public Command
{
    public:
        Object* obj;
        Object* actor;

        CmdGive();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdOpen : public Command
{
    public:
        Object* obj;

        CmdOpen();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdClose : public Command
{
    public:
        Object* obj;

        CmdClose();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdTieTo : public Command
{
    public:
        Object* tie;
        Object* tie_to;

        CmdTieTo();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdInv : public Command
{
    public:
        Object* player;

        CmdInv();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};

class CmdThrow : public Command
{
    public:
        Object* projectile;
        Object* target;

        CmdThrow();
        bool match(std::string str, std::vector<std::string>* errors, World* world);
        void run(GameState* g);
};
