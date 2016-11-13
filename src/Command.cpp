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

bool Command::parse(std::vector<std::string> tokens)
{
    return true;
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
    cmd_ptr describe = std::make_shared<CmdDescribe>();
    describe->add_object(g->engine->world->get_current_room());
    g->send_front(describe);
}

CmdDescribe::CmdDescribe(bool describe_this_in, bool deep_in)
    : Command(DESCRIBE),
    describe_this(describe_this_in), deep(deep_in)
{
}

bool CmdDescribe::parse(std::vector<std::string> tokens)
{
}

void CmdDescribe::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
    {
        Object* o = objects[i];
        if(o->properties & Object::ROOM)
        {
            g->terminal->set_color(config::colors[config::color_room_title]);
            g->terminal->disp("You in " + o->pretty_name + ".");
            g->terminal->set_color();
        }
        if(describe_this && (o->properties & Object::VISIBLE))
        {
            if (deep && !o->deep_description.empty())
                g->terminal->disp(o->deep_description);
            else
                g->terminal->disp(o->shallow_description);
        }
        if(o->properties & Object::ROOM)
        {
            for(int i = 0; i < DIRECTION_MAX; i++)
            {
                if(o->directions[i] != "")
                {
                    DirectionId dir_id = (DirectionId)i;
                    Object* dir_room = o->parent->get_direct_child(o->directions[i], 0);
                    if(dir_room && dir_room->pretty_name != "")
                    {
                        std::string dir_reference = dir[dir_id].dir_reference;
                        g->terminal->disp(dir_reference + " is " + dir_room->pretty_name + ".");
                    }
                }
            }
        }
        // If this isn't a container and show_children is true, show the children;
        // If this is a container and it's open, show the children.
        if ((!(o->properties & Object::CONTAINER) && o->show_children) || ((o->properties & Object::CONTAINER) && o->open))
        {
            for (int j = 0; j < o->children.size(); j++)
            {
                // If it's a deep description, show all children.
                // Otherwise, don't show the undiscovered children.
                if(deep || (o->children[j]->properties & Object::DISCOVERED))
                {
                    g->terminal->disp(o->children[j]->shallow_description);
                    o->children[j]->properties |= Object::DISCOVERED;
                }
            }
        }
        o->properties |= Object::DISCOVERED;
    }
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

CmdCustom::CmdCustom(std::function<void(GameState*)> fn_in)
    : Command(CUSTOM),
      fn(fn_in)
{
}

void CmdCustom::run(GameState* g)
{
    fn(g);
}
