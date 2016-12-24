#pragma once

class Object;
class GameState;
class Music;

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
        PLAY_MUSIC,
        PAUSE_MUSIC,
        STOP_MUSIC,
        // Controller commands
        INPUT,
        PAUSE,
        UNPAUSE,
        QUIT,
        // Model commands
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
        READ,
        DROP,
        PUT,
        MOVE,
        EAT,
        GIVE,
        SHOUT,
        CLOSE,
        BREAK,
        KILL,
        BLESSUP,
        DRINK,
        SMELL,
        TALK_TO,
        TIE_TO,

        ADD_GAMESTATE,
        REMOVE_GAMESTATE,
        ADD_OBJECT,
        RM_OBJECT,
        WEAR,
        HIT,
        CUSTOM
    };

    CommandType type;
    std::vector<Object*> objects;

    Command(CommandType type_in);
    virtual ~Command();

    // Adds an object to the list of objects that will be acted on
    virtual void add_object(Object* o);

    virtual void run_and_callback(GameState* g);

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
        int spread;

        CmdDisp(std::string str_in, bool append_newline_in = true);

        void run(GameState* g);
};

class CmdOutput : public Command
{
    public:
        int x;
        int y;
        int spread;
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

class CmdInput : public Command
{
    public:
        CmdInput();
        void run(GameState* g);
};

class CmdPlayMusic : public Command
{
    public:
        Music* music;
        CmdPlayMusic(Music* music_in);
        void run(GameState* g);
};

class CmdPauseMusic : public Command
{
    public:
        Music* music;
        CmdPauseMusic(Music* music_in);
        void run(GameState* g);
};

class CmdStopMusic : public Command
{
    public:
        Music* music;
        CmdStopMusic(Music* music_in);
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

class CmdAddGameState : public Command
{
    public:
        GameState* state_to_add;
        CmdAddGameState(GameState* state_to_add_in);
        void run(GameState* g);
};

class CmdRemoveGameState : public Command
{
    public:
        GameState* state_to_remove;
        CmdRemoveGameState(GameState* state_to_remove_in);
        void run(GameState* g);
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
        CmdExamine();
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

class CmdMove : public Command
{
    public:
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
