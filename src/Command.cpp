#include "Command.h"
#include "GameState.h"
#include "Engine.h"
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
    //g->engine->world->get_current_room()->describe(g);
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

CmdDescribe::CmdDescribe()
    : Command(DESCRIBE)
{
}

void CmdDescribe::describe(GameState* g, Object* o)
{
    if(o->has_component(Component::DESCRIPTION))
    {
        std::shared_ptr<ComponentDescription> cd = std::static_pointer_cast<ComponentDescription>(o->get_component(Component::DESCRIPTION));
        if(o->has_component(Component::ROOM))
        {
            g->terminal->set_color(config::colors[config::color_room_title]);
            g->terminal->disp("You in " + o->pretty_name + ".");
            g->terminal->set_color();
        }
        //if(describe_this)
        {
            if (/*deep && */!cd->deep_description.empty())
                g->terminal->disp(cd->deep_description);
            else
                g->terminal->disp(cd->shallow_description);
        }
        if(o->has_component(Component::ROOM))
        {
            std::shared_ptr<ComponentRoom> cr = std::static_pointer_cast<ComponentRoom>(o->get_component(Component::ROOM));
            for(int i = 0; i < DIRECTION_MAX; i++)
            {
                if(cr->directions[i] != "")
                {
                    DirectionId dir_id = (DirectionId)i;
                    Object* dir_room = g->engine->world->get_direct_child(cr->directions[i], 0);
                    if(dir_room && dir_room->pretty_name != "")
                    {
                        std::string dir_reference = dir[dir_id].dir_reference;
                        g->terminal->disp(dir_reference + " is " + dir_room->pretty_name + ".");
                    }
                }
            }
        }
    }
        // If this isn't a container and show_children is true, show the children;
        // If this is a container and it's open, show the children.
        /*if ((!(properties & Object::CONTAINER) && show_children) || ((properties & Object::CONTAINER) && open))
        {*/
            for (int j = 0; j < o->children.size(); j++)
            {
                // If it's a deep description, show all children.
                // Otherwise, don't show the undiscovered children.
                //if(deep)// || (o->children[j]->properties & Object::DISCOVERED))
                {
                    describe(g, o->children[j]);
                }
            }
        //}
        //properties |= Object::DISCOVERED;

}

void CmdDescribe::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
    {
        describe(g, objects[i]);
    }
}
