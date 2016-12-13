#include "Command.h"
#include "GameState.h"
#include "Engine.h"
#include "Terminal.h"
#include "World.h"
#include "Directions.h"
#include "Player.h"

Command::Command(CommandType type_in)
    : type(type_in)
{
}

Command::~Command()
{
}

void Command::run_and_callback(GameState* g)
{
    bool run_this = true;
    std::vector<Object*> callback_list = objects;

    for(int i = 0; i < callback_list.size() && run_this; i++)
    {
        if(callback_list[i]->pre_command && !callback_list[i]->pre_command(this))
            run_this = false;
    }
    if(run_this)
    {
        run(g);
        for(int i = 0; i < callback_list.size(); i++)
        {
            if(callback_list[i]->post_command)
                callback_list[i]->post_command(this);
        }
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
    g->engine->terminal->disp(str, append_newline);
}

CmdOutput::CmdOutput(int x_in, int y_in, std::string str_in)
    : Command(OUTPUT),
    x(x_in),
    y(y_in),
    str(str_in),
    spread(0)
{
}

void CmdOutput::run(GameState* g)
{
    g->engine->terminal->output(x, y, str, spread);
}

CmdClear::CmdClear()
    : Command(CLEAR)
{}

void CmdClear::run(GameState* g)
{
    g->engine->terminal->clr();
}

CmdSetColor::CmdSetColor(sf::Color color_in)
    : Command(SETCOLOR),
      color(color_in)
{
}

void CmdSetColor::run(GameState* g)
{
    g->engine->terminal->set_color(color);
}

CmdInput::CmdInput()
    : Command(INPUT)
{}

void CmdInput::run(GameState* g)
{
    g->engine->terminal->input_mode();
}

CmdPlayMusic::CmdPlayMusic(Music* music_in)
    : Command(PLAY_MUSIC),
      music(music_in)
{}

void CmdPlayMusic::run(GameState* g)
{
    g->engine->audio->play_music(music);
}

CmdPauseMusic::CmdPauseMusic(Music* music_in)
    : Command(PAUSE_MUSIC),
    music(music_in)
{}

void CmdPauseMusic::run(GameState* g)
{
    music->set_fade(Music::PAUSE);
}

CmdStopMusic::CmdStopMusic(Music* music_in)
    : Command(STOP_MUSIC),
    music(music_in)
{}

void CmdStopMusic::run(GameState* g)
{
    music->set_fade(Music::STOP);
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

CmdRemoveGameState::CmdRemoveGameState(GameState* state_to_remove_in)
    : Command(REMOVE_GAMESTATE),
    state_to_remove(state_to_remove_in)
{}

void CmdRemoveGameState::run(GameState* g)
{
    state_to_remove->running = false;
}

CmdGo::CmdGo(std::string new_room_in)
    : Command(GO),
      new_room(new_room_in)
{
}

void CmdGo::run(GameState* g)
{
    ComponentMusic* music_leaving = (ComponentMusic*)g->world->get_current_room()->get_component(Component::MUSIC);
    g->world->set_current_room(new_room);
    ComponentMusic* music_entering = (ComponentMusic*)g->world->get_current_room()->get_component(Component::MUSIC);
    if(music_leaving && !music_leaving->persistent)
        g->send_front(std::make_shared<CmdPauseMusic>(music_leaving->music));
    if(music_entering)
        g->send_front(std::make_shared<CmdPlayMusic>(music_entering->music));
    //g->world->get_current_room()->describe(g);
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
    : Command(DESCRIBE),
    deep(false)
{
}

void CmdDescribe::describe(GameState* g, Object* o, bool deep_describe)
{
    ComponentRoom* c_room = (ComponentRoom*)o->get_component(Component::ROOM);
    ComponentDescription* c_desc = (ComponentDescription*)o->get_component(Component::DESCRIPTION);
    ComponentText* c_text = (ComponentText*)o->get_component(Component::TEXT);

    if(c_room)
    {
        g->engine->terminal->disp("You are in " + o->pretty_name + ".");
        if(c_desc)
            g->engine->terminal->disp(c_desc->current_appearance);
    }
    else if(c_desc)
    {
        if(deep_describe)
        {
            if(c_desc->description != "")
            {
                g->engine->terminal->disp(c_desc->description);
            }
            else if(c_text)
            {
                std::shared_ptr<CmdRead> read = std::make_shared<CmdRead>();
                read->add_object(o);
                g->send_front(read);
            }
            else if(o->children.empty())
            {
                g->engine->terminal->disp("You see nothing special about the " + o->pretty_name + ".");
            }
        }
        else
        {
            g->engine->terminal->disp(c_desc->current_appearance);
        }
    }

    for (int j = 0; j < o->children.size(); j++)
    {
        // If it's a deep description, show all children.
        // Otherwise, don't show the undiscovered children.
        if(o->children[j]->active &&
            (deep_describe || o->children[j]->discovered))
        {
            describe(g, o->children[j]);
        }
    }
    if(c_room)
    {
        for(int i = 0; i < DIRECTION_MAX; i++)
        {
            if(c_room->directions[i] != "")
            {
                DirectionId dir_id = (DirectionId)i;
                Object* dir_room = g->world->get_direct_child(c_room->directions[i], 0);
                if(dir_room && dir_room->pretty_name != "")
                {
                    std::string dir_reference = dir[dir_id].dir_reference;
                    g->engine->terminal->disp(dir_reference + " is " + dir_room->pretty_name + ".");
                }
            }
        }
    }
    o->discovered = true;
}

void CmdDescribe::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
    {
        describe(g, objects[i], deep);
    }
}

CmdTake::CmdTake()
    : Command(TAKE)
{ }

void CmdTake::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
    {
        if(objects[i]->parent)
            objects[i]->parent->remove_child(objects[i]);
        if(g->world->get_player())
            g->world->get_player()->add_child(objects[i]);
    }
}

CmdHit::CmdHit()
    : Command(HIT)
{ }

void CmdHit::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
    {
        ComponentHittable* c_hittable = (ComponentHittable*)objects[i]->get_component(Component::HITTABLE);
        if(c_hittable)
            c_hittable->flipped = !c_hittable->flipped;
    }
}

CmdWear::CmdWear()
    : Command(WEAR)
{ }

void CmdWear::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
    {
        if(objects[i]->parent)
            objects[i]->parent->remove_child(objects[i]);
        if(g->world->get_player())
        {
            g->world->get_player()->add_child(objects[i]);
            ((Player*)g->world->get_player())->clothing = objects[i]->name;
        }
    }
}

CmdRead::CmdRead()
    : Command(READ)
{ }

void CmdRead::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
    {
        ComponentText* c_text = (ComponentText*)objects[i]->get_component(Component::TEXT);
        if(c_text)
        {
            g->send_front(std::make_shared<CmdDisp>("The " + objects[i]->pretty_name + " reads:"));
            g->send_front(std::make_shared<CmdDisp>(c_text->text));
        }
        else
        {
            g->send_front(std::make_shared<CmdDisp>("There's nothing to read on the " + objects[i]->pretty_name + "."));
        }
    }
}
