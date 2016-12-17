#include "Command.h"
#include "GameState.h"
#include "Engine.h"
#include "Terminal.h"
#include "World.h"
#include "Directions.h"
#include "Player.h"
#include <set>

Command::Command(CommandType type_in)
    : type(type_in)
{
}

Command::~Command()
{
}

void Command::run_and_callback(GameState* g)
{
    std::set<Object*> callback_list;
    for(int i = 0; i < std::max(1, int(objects.size())); i++)
    {
        Object* object = objects.size() > i ? objects[i] : g->world->get_player();
        if(object)
        {
            while(object)
            {
                callback_list.insert(object);
                object = object->parent;
            }
        }
    }

    bool run_this = true;
    for(auto it = callback_list.rbegin(); it != callback_list.rend() && run_this; ++it)
    {
        if((*it)->pre_command && !(*it)->pre_command(this))
            run_this = false;
    }
    if(run_this)
    {
        run(g);
        for(auto it = callback_list.rbegin(); it != callback_list.rend(); ++it)
        {
            if((*it)->post_command)
                (*it)->post_command(this);
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
    if(g->world->get_indirect_child(new_room, 0))
    {
        ComponentMusic* music_leaving = (ComponentMusic*)g->world->get_current_room()->get_component(Component::MUSIC);
        g->world->set_current_room(new_room);
        ComponentMusic* music_entering = (ComponentMusic*)g->world->get_current_room()->get_component(Component::MUSIC);
        if(music_leaving && !music_leaving->persistent)
            g->send_front(std::make_shared<CmdPauseMusic>(music_leaving->music));
        if(music_entering)
            g->send_front(std::make_shared<CmdPlayMusic>(music_entering->music));

        std::shared_ptr<CmdLookAround> look_around = std::make_shared<CmdLookAround>();
        g->send_front(look_around);
    }
    else
    {
        g->send_front(std::make_shared<CmdDisp>("Error: room " + new_room + " doesn't exist."));
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

CmdCustom::CmdCustom(std::function<void(GameState*)> fn_in)
    : Command(CUSTOM),
      fn(fn_in)
{
}

void CmdCustom::run(GameState* g)
{
    fn(g);
}

CmdLookAround::CmdLookAround()
    : Command(LOOK_AROUND)
{
}

void recursive_show(GameState* g, Object* o, bool show_children, bool description)
{
    ComponentDescription* c_desc = (ComponentDescription*)o->get_component(Component::DESCRIPTION);
    ComponentText* c_text = (ComponentText*)o->get_component(Component::TEXT);
    ComponentInventory* c_inv = (ComponentInventory*)o->get_component(Component::INVENTORY);
    ComponentOpenClose* c_open = (ComponentOpenClose*)o->get_component(Component::OPEN_CLOSE);
    ComponentTie* c_tie = (ComponentTie*)o->get_component(Component::TIE);
    ComponentTieTo* c_tie_to = (ComponentTieTo*)o->get_component(Component::TIE_TO);

    if(c_desc && c_desc->current_appearance != "")
    {
        if(description)
        {
            if(c_desc->description != "")
                g->engine->terminal->disp(c_desc->description);
            if(c_text)
                g->engine->terminal->disp("The " + o->pretty_name + " reads:\n" + c_text->text);
            if(c_open)
                g->engine->terminal->disp(std::string("It is ") + (c_open->open ? "open" : "closed") + ".");
            if(o->children.size() == 0 && !c_text && !c_open)
                g->engine->terminal->disp("You see nothing special about the " + o->pretty_name + ".");
        }
        else
            g->engine->terminal->disp(c_desc->current_appearance);
    }

    if(!c_inv && (!c_open || c_open->open))
    {
        for (int j = 0; j < o->children.size(); j++)
        {
            if(o->children[j]->active && (show_children || o->children[j]->discovered))
                recursive_show(g, o->children[j]);
        }
    }

    o->discovered = true;
}

void CmdLookAround::run(GameState* g)
{
    Object* player = g->world->get_player();
    if(player)
    {
        Object* room = player->parent;
        if(room)
        {
            g->engine->terminal->disp("Your location: " + room->pretty_name + ".");
            recursive_show(g, room, true, false);
            ComponentRoom* c_room = (ComponentRoom*)room->get_component(Component::ROOM);
            if(c_room)
            {
                std::vector<DirectionId> unexplored_exits = {};
                bool explored_exits_exist = false;
                for(int i = 0; i < DIRECTION_MAX; i++)
                {
                    DirectionId dir_id = (DirectionId)i;
                    if(c_room->directions.count(dir_id) > 0 && c_room->hidden.count(dir_id) == 0)
                    {
                        Object* dir_room = g->world->get_direct_child(c_room->directions[dir_id], 0);
                        if(dir_room)
                        {
                            std::string dir_reference = dir[dir_id].dir_reference;
                            if(dir_room->discovered)
                            {
                                explored_exits_exist = true;
                                g->engine->terminal->disp(dir_reference + " is " + dir_room->pretty_name + ".");
                            }
                            else
                                unexplored_exits.push_back(dir_id);
                        }
                    }
                }
                if(unexplored_exits.size() > 0)
                {
                    std::string output = "You can " + std::string(explored_exits_exist ? "also " : "") + "go ";
                    for(int i = 0; i < unexplored_exits.size(); i++)
                    {
                        output += dir[unexplored_exits[i]].name;
                        if(i < (int)unexplored_exits.size() - 2)
                            output += ", ";
                        else if(i == unexplored_exits.size() - 2)
                        {
                            if(unexplored_exits.size() == 2)
                                output += " or ";
                            else
                                output += ", or ";
                        }
                    }
                    output += ".";
                    g->engine->terminal->disp(output);
                }
            }
        }
    }
}

CmdExamine::CmdExamine()
    : Command(EXAMINE)
{ }

void CmdExamine::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
        recursive_show(g, objects[i], true, true);
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
        g->engine->terminal->disp("You take the " + objects[i]->pretty_name + ".");
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
        g->engine->terminal->disp("You put on the " + objects[i]->pretty_name + ".");
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

CmdMove::CmdMove()
    : Command(MOVE)
{ }

void CmdMove::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
    {
        ComponentMoveable* c_move = (ComponentMoveable*)objects[i]->get_component(Component::MOVEABLE);
        ComponentDescription* c_desc = (ComponentDescription*)objects[i]->get_component(Component::DESCRIPTION);
        if(c_move)
        {
            if(c_move->new_parent)
            {
                objects[i]->parent->remove_child(objects[i]);
                c_move->new_parent->add_child(objects[i]);
            }
            if(c_desc)
                c_desc->current_appearance = "There is a " + objects[i]->pretty_name + " here.";
            g->engine->terminal->disp("With considerable effort, you move the " + objects[i]->pretty_name + " aside.");
        }
        else
        {
            g->engine->terminal->disp("Try as you might, the " + objects[i]->pretty_name + " will not budge.");
        }
    }
}

CmdTalkTo::CmdTalkTo()
    : Command(TALK_TO)
{ }

void CmdTalkTo::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
    {
        ComponentTalkable* c_talk = (ComponentTalkable*)objects[i]->get_component(Component::TALKABLE);
        if(c_talk)
        {
           for(auto j = c_talk->talkable_data.begin(); j != c_talk->talkable_data.end(); j++) 
           {
               bool other = (j->size() > 0 && (*j)[0] == '-');
               std::string output_text = "("
                   + (other ? objects[i]->pretty_name : g->world->get_player()->pretty_name)
                   + ") "
                   + (other ? j->substr(1, j->size() - 1) : *j);
               g->send_front(std::make_shared<CmdDisp>(output_text));
               g->send_front(std::make_shared<CmdPause>());
           }
        }
        else
        {
            g->send_front(std::make_shared<CmdDisp>(">he thinks he can talk to a " + objects[i]->pretty_name));
        }
    }
}

CmdEat::CmdEat(Object* food_in)
    : Command(EAT),
    food(food_in)
{
    objects.push_back(food);
}

void CmdEat::run(GameState* g)
{
    if(!food)
    {
        g->send_front(std::make_shared<CmdDisp>("Eat what?"));
    }
    else if(!food->has_component(Component::EDIBLE))
    {
        g->send_front(std::make_shared<CmdDisp>("The " + food->pretty_name + " isn't edible."));
    }
    else
    {
        g->send_front(std::make_shared<CmdDisp>("You eat the " + food->pretty_name + " and find it most satisfactory."));
        if(food->parent)
            food->parent->remove_child(food);
        food->active = false;
    }
}

CmdFeed::CmdFeed(Object* food_in, Object* actor_in)
    : Command(FEED),
    food(food_in),
    actor(actor_in)
{
    objects.push_back(food);
    objects.push_back(actor);
}

void CmdFeed::run(GameState* g)
{
    if(!food)
    {
        g->send_front(std::make_shared<CmdDisp>("Food not found."));
    }
    else if(!actor)
    {
        g->send_front(std::make_shared<CmdDisp>("Actor not found."));
    }
    else if(!food->has_component(Component::EDIBLE))
    {
        g->send_front(std::make_shared<CmdDisp>("A " + food->pretty_name + " isn't edible."));
    }
    else if(!actor->has_component(Component::TALKABLE))
    {
        g->send_front(std::make_shared<CmdDisp>(">implying one can feed a " + food->pretty_name + " to a " + actor->pretty_name));
    }
    else
    {
        g->send_front(std::make_shared<CmdDisp>("You feed the " + food->pretty_name + " to " + actor->pretty_name + "."));
        if(food->parent)
            food->parent->remove_child(food);
        food->active = false;
    }
}

CmdOpen::CmdOpen(Object* obj_in)
    : Command(OPEN),
    obj(obj_in)
{
    objects.push_back(obj);
}

void CmdOpen::run(GameState* g)
{
    if(!obj)
    {
        g->send_front(std::make_shared<CmdDisp>("No object to open"));
    }
    else
    {
       ComponentOpenClose* c_open_close = (ComponentOpenClose*)obj->get_component(Component::OPEN_CLOSE);
       
        if(!c_open_close)
        {
            g->send_front(std::make_shared<CmdDisp>("You can't open the " + obj->pretty_name + "!"));
        }
        else if(c_open_close->open)
        {
            g->send_front(std::make_shared<CmdDisp>("The " + obj->pretty_name + " is already open."));
        }
        else
        {
            c_open_close->open = true;
            g->send_front(std::make_shared<CmdDisp>("You open the " + obj->pretty_name + "."));
        }
    }
}

CmdTieTo::CmdTieTo(Object* tie_in, Object* tie_to_in)
    : Command(TIE_TO),
    tie(tie_in),
    tie_to(tie_to_in)
{
}

void CmdTieTo::run(GameState* g)
{
    if(tie && tie_to)
    {
        if(tie->pre_command(this) && tie_to->pre_command(this))
        {
            ComponentTie* c_tie = (ComponentTie*)tie->get_component(Component::TIE);
            ComponentTieTo* c_tie_to = (ComponentTieTo*)tie_to->get_component(Component::TIE_TO);
            ComponentDescription* c_desc = (ComponentDescription*)tie->get_component(Component::DESCRIPTION);

            if(c_tie && c_tie_to)
            {
                c_tie->tie_to.push_back(tie_to);
                c_tie_to->tie.push_back(tie);
                if(tie->parent)
                    tie->parent->remove_child(tie);
                tie_to->add_child(tie);
                if(c_desc)
                    c_desc->current_appearance = "There is a " + tie->pretty_name + " tied to the " + tie_to->pretty_name + ".";
                g->engine->terminal->disp("You tie the " + tie->pretty_name + " to the " + tie_to->pretty_name + ".");
            }
        }
    }
}
