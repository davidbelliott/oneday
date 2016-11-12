#include "Command.h"
#include "GameState.h"
#include "Engine.h"
#include "Room.h"
#include "Terminal.h"
#include "World.h"

Command::Command(CommandType type_in)
    : type(type_in)
{
}

Command::~Command()
{
}

void Command::run(GameState* g)
{
}

void Command::add_object(Object* o)
{
    objects.push_back(o);
}

CmdDisp::CmdDisp(std::string str_in)
:   Command(DISP),
    str(str_in)
{
}

void CmdDisp::run(GameState* g)
{
    g->terminal->disp(str);
}

CmdOutput::CmdOutput(int x_in, int y_in, std::string str_in)
    : Command(OUTPUT),
    x(x_in),
    y(y_in),
    str(str_in)
{
}

void CmdOutput::run(GameState* g)
{
    g->terminal->output(x, y, str);
}

CmdClear::CmdClear()
    : Command(CLEAR)
{}

void CmdClear::run(GameState* g)
{
    g->terminal->clr();
}

CmdSetColor::CmdSetColor(sf::Color color_in)
    : Command(SETCOLOR),
      color(color_in)
{
}

void CmdSetColor::run(GameState* g)
{
    g->terminal->set_color(color);
}

CmdInput::CmdInput()
    : Command(INPUT)
{}

void CmdInput::run(GameState* g)
{
}

CmdPause::CmdPause()
    : Command(PAUSE)
{}

void CmdPause::run(GameState* g)
{
    g->pause();
}

CmdUnpause::CmdUnpause()
    : Command(UNPAUSE)
{}

void CmdUnpause::run(GameState* g)
{
    g->unpause();
}

CmdSetObjective::CmdSetObjective(std::string objective_in)
    : Command(SET_OBJECTIVE),
      objective(objective_in)
{}

void CmdSetObjective::run(GameState* g)
{

}

CmdAddGameState::CmdAddGameState(GameState* state_to_add_in)
    : Command(ADD_GAMESTATE),
      state_to_add(state_to_add_in)
{}

void CmdAddGameState::run(GameState* g)
{

}

CmdSetRoom::CmdSetRoom(World* world_in, std::string new_room_in)
    : Command(SET_ROOM),
      world(world_in),
      new_room(new_room_in)
{
}

void CmdSetRoom::run(GameState* g)
{
    world->set_current_room(new_room);
    world->get_current_room()->describe(g, true, true);
}

CmdDescribe::CmdDescribe()
    : Command(DESCRIBE)
{
}

void CmdDescribe::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
        objects[i]->describe(g, true, true);
}

CmdQuit::CmdQuit()
    : Command(QUIT)
{
}

void CmdQuit::run(GameState* g)
{
    g->engine->running = false;
}

CmdTake::CmdTake()
    : Command(TAKE)
{
}

void CmdTake::run(GameState* g)
{
    
}

CmdWear::CmdWear()
    : Command(WEAR)
{
}

void CmdWear::run(GameState* g)
{

}

CmdHit::CmdHit()
    : Command(HIT)
{
}

void CmdHit::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
    {
        Object* o = objects[i];

        if (o->properties & Object::HITTABLE)
        {
            if (o->flipped)
                g->send(std::make_shared<CmdDisp>("The " + o->name.word + " has already been hit."));
            else
            {
                g->send(std::make_shared<CmdDisp>("You hit the " + o->name.word + "."));
                o->flipped = true;
            }
        }
        else if (o)
        {
            g->send(std::make_shared<CmdDisp>("You can't hit the " + o->name.word));
        }
    }
}

CmdShout::CmdShout()
    : Command(SHOUT)
{
}

void CmdShout::run(GameState* g)
{
    g->terminal->disp("-SHEEEEIT!");
}

CmdRead::CmdRead()
    : Command(READ)
{
}

void CmdRead::run(GameState* g)
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

CmdTalkTo::CmdTalkTo()
    : Command(TALK_TO)
{
}

void CmdTalkTo::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
    {
        Object* o = objects[i];
        if(o->properties & Object::TALKABLE)
        {
            for(int j = 0; j < o->talkable_data.size(); j++)
            {
                g->send(std::make_shared<CmdDisp>(o->talkable_data[i]));
                g->send(std::make_shared<CmdPause>());
            }
        }
    }
}

CmdHelp::CmdHelp()
    : Command(HELP)
{
}

void CmdHelp::run(GameState* g)
{

}
