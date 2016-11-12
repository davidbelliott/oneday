#pragma once

#include "Object.h"
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <memory>

typedef int cmd_ptr;

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

    Command(CommandType type_in)
    : type(type_in)
    {
    }

    virtual ~Command()
    {
    }

    virtual void run(GameState* g)
    {
    }

    virtual void add_object(Object* o)
    {
        objects.push_back(o);
    }
};

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

        void run(GameState* g)
        {
            g->terminal->disp(str);
        }
};

class CmdOutput : public Command
{
    public:
        int x;
        int y;
        std::string str;

        CmdOutput(int x_in, int y_in, std::string str_in)
            : Command(CMD_OUTPUT),
            x(x_in),
            y(y_in),
            str(str_in)
        {
        }

        void run(GameState* g)
        {
            g->terminal->output(x, y, str);
        }
};

class CmdClear : public Command
{
    public:
        CmdClear()
            : Command(CMD_CLEAR)
        {}

        void run(GameState* g)
        {
            g->terminal->clr();
        }
};

class CmdSetColor : public Command
{
    public:
        sf::Color color;
        CmdSetColor(sf::Color color_in = config::colors[config::color_default_fg])
            : Command(CMD_SETCOLOR),
              color(color_in)
        {}

        void run(GameState* g)
        {
            g->terminal->set_color(color);
        }
};

class CmdInput : public Command
{
    public:
        CmdInput()
            : Command(CMD_INPUT)
        {}

        void run(GameState* g)
        {

        }
};

class CmdPause : public Command
{
    public:
        CmdPause()
            : Command(CMD_PAUSE)
        {}

        void run(GameState* g)
        {
            g->pause();
        }
};

class CmdUnpause : public Command
{
    public:
        CmdUnpause()
            : Command(CMD_UNPAUSE)
        {}

        void run(GameState* g)
        {
            g->unpause();
        }
};

class CmdSetObjective : public Command
{
    public:
        std::string objective;
        CmdSetObjective(std::string objective_in)
            : Command(CMD_SET_OBJECTIVE),
              objective(objective_in)
        {}

        void run(GameState* g)
        {

        }
};

class CmdAddGameState : public Command
{
    public:
        GameState* state_to_add;
        CmdAddGameState(GameState* state_to_add_in)
            : Command(CMD_ADD_GAMESTATE),
              state_to_add(state_to_add_in)
        {}

        void run(GameState* g)
        {

        }
};

class CmdSetRoom : public Command
{
    public:
        World* world;
        std::string new_room;
        CmdChangeRoom(World* world_in, std::string new_room_in)
            : Command(CMD_SET_ROOM),
              world(world_in),
              new_room(new_room_in)
        {
        }

        void run(GameState* g)
        {
            world->set_room(new_room);
        }
};

class CmdDescribe : public Command
{
    public:
        CmdDescribe()
            : Command(CMD_DESCRIBE)
        {
        }

        void run(GameState* g)
        {
            for(int i = 0; i < objects.size(); i++)
                objects[i]->describe(g, true, true);
        }
};

class CmdQuit : public Command
{
    public:
        CmdQuit()
            : Command(CMD_QUIT)
        {
        }

        void run(GameState* g)
        {
            g->engine->running = false;
        }
};

class CmdTake : public Command
{
    public:
        CmdTake()
            : Command(CMD_TAKE)
        {
        }

        void run(GameState* g)
        {
            
        }
};

class CmdWear : public Command
{
    public:
        CmdWear()
            : Command(CMD_WEAR)
        {
        }

        void run(GameState* g)
        {

        }
};

class CmdHit : public Command
{
    public:
        CmdHit()
            : Command(CMD_HIT)
        {
        }

        void run(GameState* g)
        {
            for(int i = 0; i < objects.size(); i++)
            {
                Object* o = objects[i];

                if (o->properties & Object::HITTABLE)
                {
                    if (o->flipped)
                        r->add_event(std::make_shared<CmdDisp>("The " + o->name.word + " has already been hit."));
                    else
                    {
                        r->add_event(std::make_shared<CmdDisp>("You hit the " + o->name.word + "."));
                        o->flipped = true;
                    }
                }
                else if (o)
                {
                    r->add_event(std::make_shared<CmdDisp>("You can't hit the " + o->name.word));
                }
            }
        }
};

class CmdShout : public Command
{
    public:
        CmdShout()
            : Command(CMD_SHOUT)
        {
        }

        void run(GameState* g)
        {
            g->terminal->disp("-SHEEEEIT!");
        }
};

class CmdRead : public Command
{
    public:
        CmdRead()
            : Command(CMD_READ)
        {
        }

        void run(GameState* g)
        {
            for(int i = 0; i < objects.size(); i++)
            {
                if(objects[i]->properties & Object::READABLE)
                {
                    g->terminal->disp("The " + objects[i]->name.word + " reads:");
                    g->terminal->disp(objects[i]->readable_data);
                }
            }
        }
};

class CmdTalkTo : public Command
{
    public:
        CmdTalkTo()
            : Command(CMD_TALK_TO)
        {
        }

        void run(GameState* g)
        {
            for(int i = 0; i < objects[i].size(); i++)
            {
                Object* o = objects[i];
                if(o->properties & Object::TALKABLE)
                {
                    for(int j = 0; j < o->talkable_data.size(); j++)
                    {
                        g->send(std::make_shared<CmdDisp>(o->talkable_data[i]));
                        o->send(std::make_shared<CmdPause>());
                    }
                }
            }
        }
};

class CmdHelp : public Command
{
    public:
        CmdHelp()
            : Command(CMD_HELP)
        {
        }

        void run(GameState* g)
        {

        }
};
