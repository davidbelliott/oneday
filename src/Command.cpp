#include "Command.h"
#include "GameState.h"
#include "Engine.h"
#include "Room.h"
#include "Terminal.h"
#include "World.h"
#include "Directions.h"

Command::Command(CommandType type_in)
    : type(type_in),
      patterns({})
{
}

Command::~Command()
{
}

void Command::run_with_callbacks(GameState* g)
{
    for(int i = 0; i < objects.size(); )
    {
        auto pre_command = objects[i]->pre_command;
        bool remove_this_object = false;
        if(pre_command)
        {
            if(!pre_command(this))
                remove_this_object = true;
        }
        if(remove_this_object)
            objects.erase(objects.begin() + i);
        else
            i++;
    }
    run(g);
    for(int i = 0; i < objects.size(); i++)
    {
        auto post_command = objects[i]->post_command;
        if(post_command)
            post_command(this);
    }
}

void Command::run(GameState* g)
{
}

void Command::add_object(Object* o)
{
    objects.push_back(o);
}

CmdDisp::CmdDisp(std::string str_in, bool append_newline_in)
:   Command(DISP),
    str(str_in),
    append_newline(append_newline_in)
{
}

void CmdDisp::run(GameState* g)
{
    g->terminal->disp(str, append_newline);
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
    g->terminal->input_mode();
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
    g->engine->push_state(state_to_add);
}

CmdSetRoom::CmdSetRoom(std::string new_room_in)
    : Command(SET_ROOM),
      new_room(new_room_in)
{
}

void CmdSetRoom::run(GameState* g)
{
    g->engine->world->set_current_room(new_room);
    g->engine->world->get_current_room()->describe(g);
}

CmdQuit::CmdQuit()
    : Command(QUIT)
{
}

void CmdQuit::run(GameState* g)
{
    g->engine->running = false;
}

CmdCustom::CmdCustom(std::function<void(GameState*)> fn_in)
    : Command(CUSTOM),
      fn(fn_in)
{
}

void CmdCustom::run(GameState* g)
{
    fn(g);
}
