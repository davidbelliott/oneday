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

/*void Command::run_and_callback(GameState* g)
{
    std::set<Object*> callback_list;
    for(int i = 0; i < std::max(1, int(objects.size())); i++)
    {
        Object* object = objects.size() > i ? objects[i] : (g->world ? g->world->get_player() : nullptr);
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
}*/

void Command::run(GameState* g)
{

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
        /*if(music_leaving)
            g->engine->audio->pause_music(music_leaving->music);
        if(music_entering)
            g->engine->audio->play_music(music_entering->music, music_entering->start_time);*/

        CmdLookAround look_around;
        look_around.run(g);
    }
    else
    {
        g->engine->terminal->disp("Error: room " + new_room + " doesn't exist.");
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

CmdLookAround::CmdLookAround()
    : Command(LOOK_AROUND)
{
}

void recursive_show(GameState* g, Object* o, bool show_children, bool appearance, bool description)
{
    ComponentDescription* c_desc = (ComponentDescription*)o->get_component(Component::DESCRIPTION);
    ComponentText* c_text = (ComponentText*)o->get_component(Component::TEXT);
    ComponentInventory* c_inv = (ComponentInventory*)o->get_component(Component::INVENTORY);
    ComponentOpenClose* c_open = (ComponentOpenClose*)o->get_component(Component::OPEN_CLOSE);
    ComponentTie* c_tie = (ComponentTie*)o->get_component(Component::TIE);
    ComponentTieTo* c_tie_to = (ComponentTieTo*)o->get_component(Component::TIE_TO);

    if(c_desc && c_desc->current_appearance != "")
    {
        if(appearance)
            g->engine->terminal->disp(c_desc->current_appearance);
        if(description)
        {
            if(c_desc->description != "")
                g->engine->terminal->disp(c_desc->description);
            if(c_text)
                g->engine->terminal->disp("The " + o->pretty_name + " reads:\n" + c_text->text);
            if(c_open)
                g->engine->terminal->disp(std::string("It is ") + (c_open->open ? "open" : "closed") + ".");
            if(c_desc->description == "" && o->children.size() == 0 && !c_text && !c_open)
                g->engine->terminal->disp("You see nothing special about the " + o->pretty_name + ".");
        }
    }

    if(!c_inv && (!c_open || c_open->open))
    {
        for (int j = 0; j < o->children.size(); j++)
        {
            if(o->children[j]->active && (show_children || o->children[j]->discovered))
                recursive_show(g, o->children[j], false, true, false);
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
            recursive_show(g, room, true, true, false);
            ComponentRoom* c_room = (ComponentRoom*)room->get_component(Component::ROOM);
            if(c_room)
            {
                std::vector<DirectionId> unexplored_exits = {};
                bool explored_exits_exist = false;
                for(int i = 0; i < DIRECTION_MAX; i++)
                {
                    DirectionId dir_id = (DirectionId)i;
                    if(c_room->directions.count(dir_id) > 0 && c_room->directions[dir_id] != "" && c_room->hidden.count(dir_id) == 0)
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
    recursive_show(g, object, true, false, true);
}

CmdTake::CmdTake()
    : Command(TAKE)
{ }

void CmdTake::run(GameState* g)
{
    if(object->parent == g->world->get_player())
    {
        g->engine->terminal->disp("You already have the " + object->pretty_name + ".");
    }
    else
    {
        if(object->parent)
            object->parent->remove_child(object);
        if(g->world->get_player())
            g->world->get_player()->add_child(object);
        g->engine->terminal->disp("You take the " + object->pretty_name + ".");
    }
}

CmdHit::CmdHit()
    : Command(HIT)
{ }

void CmdHit::run(GameState* g)
{
    ComponentHittable* c_hittable = (ComponentHittable*)object->get_component(Component::HITTABLE);
    if(c_hittable)
        c_hittable->flipped = !c_hittable->flipped;
}

CmdWear::CmdWear()
    : Command(WEAR)
{ }

void CmdWear::run(GameState* g)
{
    if(object->parent)
        object->parent->remove_child(object);
    if(g->world->get_player())
    {
        g->world->get_player()->add_child(object);
        ((Player*)g->world->get_player())->clothing = object->name;
    }
    g->engine->terminal->disp("You put on the " + object->pretty_name + ".");
}

CmdRead::CmdRead()
    : Command(READ)
{ }

void CmdRead::run(GameState* g)
{
    ComponentText* c_text = (ComponentText*)object->get_component(Component::TEXT);
    if(c_text)
    {
        g->engine->terminal->disp("The " + object->pretty_name + " reads:");
        g->engine->terminal->disp(c_text->text);
    }
    else
    {
        g->engine->terminal->disp("There's nothing to read on the " + object->pretty_name + ".");
    }
}

CmdMove::CmdMove()
    : Command(MOVE)
{ }

void CmdMove::run(GameState* g)
{
    ComponentMoveable* c_move = (ComponentMoveable*)object->get_component(Component::MOVEABLE);
    ComponentDescription* c_desc = (ComponentDescription*)object->get_component(Component::DESCRIPTION);
    if(c_move)
    {
        if(c_move->new_parent)
        {
            object->parent->remove_child(object);
            c_move->new_parent->add_child(object);
        }
        if(c_desc)
            c_desc->current_appearance = "There is a " + object->pretty_name + " here.";
        g->engine->terminal->disp("With considerable effort, you move the " + object->pretty_name + " aside.");
    }
    else
    {
        g->engine->terminal->disp("Try as you might, the " + object->pretty_name + " will not budge.");
    }
}

CmdTalkTo::CmdTalkTo()
    : Command(TALK_TO)
{ }

void CmdTalkTo::run(GameState* g)
{
    /*ComponentTalkable* c_talk = (ComponentTalkable*)object->get_component(Component::TALKABLE);
    if(c_talk)
    {
       for(auto j = c_talk->talkable_data.begin(); j != c_talk->talkable_data.end(); j++) 
       {
           bool other = (j->size() > 0 && (*j)[0] == '-');
           std::string output_text = "("
               + (other ? object->pretty_name : g->world->get_player()->pretty_name)
               + ") "
               + (other ? j->substr(1, j->size() - 1) : *j);
           g->engine->terminal->disp(output_text);
           g->engine->terminal->pause();
       }
    }
    else
    {
        g->terminal->disp(">he thinks he can talk to a " + object->pretty_name));
    }*/
}

CmdEat::CmdEat(Object* food_in)
    : Command(EAT),
    food(food_in)
{
}

void CmdEat::run(GameState* g)
{
    if(!food)
    {
        g->engine->terminal->disp("Eat what?");
    }
    else if(!food->has_component(Component::EDIBLE))
    {
        g->engine->terminal->disp("The " + food->pretty_name + " isn't edible.");
    }
    else
    {
        g->engine->terminal->disp("You eat the " + food->pretty_name + " and find it most satisfactory.");
        if(food->parent)
            food->parent->remove_child(food);
        food->active = false;
    }
}

CmdGive::CmdGive(Object* obj_in, Object* actor_in)
    : Command(GIVE),
    obj(obj_in),
    actor(actor_in)
{

}

void CmdGive::run(GameState* g)
{
    if(!obj)
        g->engine->terminal->disp("Object not found.");
    else if(!actor)
        g->engine->terminal->disp("Actor not found.");
    else if(!obj->has_component(Component::TAKEABLE))
        g->engine->terminal->disp("A " + obj->pretty_name + " can't be taken.");
    else if(!actor->has_component(Component::TALKABLE))
        g->engine->terminal->disp(">implying one can give a " + obj->pretty_name + " to a " + actor->pretty_name);
    else
    {
        g->engine->terminal->disp("You give the " + obj->pretty_name + " to " + actor->pretty_name + ".");
        if(obj->parent)
            obj->parent->remove_child(obj);
        obj->active = false;
    }
}

CmdOpen::CmdOpen(Object* obj_in)
    : Command(OPEN),
    obj(obj_in)
{

}

void CmdOpen::run(GameState* g)
{
    if(!obj)
        g->engine->terminal->disp("No object to open");
    else
    {
       ComponentOpenClose* c_open_close = (ComponentOpenClose*)obj->get_component(Component::OPEN_CLOSE);
       
        if(!c_open_close)
            g->engine->terminal->disp("You can't open the " + obj->pretty_name + "!");
        else if(c_open_close->open)
            g->engine->terminal->disp("The " + obj->pretty_name + " is already open.");
        else
        {
            c_open_close->open = true;
            g->engine->terminal->disp("You open the " + obj->pretty_name + ".");
            recursive_show(g, obj, true, false, false);
        }
    }
}

CmdClose::CmdClose(Object* obj_in)
    : Command(CLOSE),
    obj(obj_in)
{
}

void CmdClose::run(GameState* g)
{
    if(!obj)
        g->engine->terminal->disp("No object to close");
    else
    {
       ComponentOpenClose* c_open_close = (ComponentOpenClose*)obj->get_component(Component::OPEN_CLOSE);
       
        if(!c_open_close)
            g->engine->terminal->disp("You can't close the " + obj->pretty_name + "!");
        else if(!c_open_close->open)
            g->engine->terminal->disp("The " + obj->pretty_name + " is already closed.");
        else
        {
            c_open_close->open = false;
            g->engine->terminal->disp("You close the " + obj->pretty_name + ".");
            //recursive_show(g, obj, true, false, false);
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
            tie->post_command(this);
            tie_to->post_command(this);
        }
    }
}

CmdInv::CmdInv(Object* player_in)
    : Command(INVENTORY),
    player(player_in)
{
}

void CmdInv::run(GameState* g)
{
    if(player && player->has_component(Component::INVENTORY))
    {
        std::string clothing_name = ((Player*)player)->clothing;
        g->engine->terminal->disp(std::string("You are carrying")
                + (player->children.size() > 0 ? ":" : " nothing."));
        for(int i = 0; i < player->children.size(); i++)
        {
            std::string output = " * " + player->children[i]->pretty_name
                + ((clothing_name != "" && clothing_name == player->children[i]->name) ?
                        " (wearing)" : "");
            g->engine->terminal->disp(output);
        }
    }
}

CmdThrow::CmdThrow(Object* projectile_in, Object* target_in)
    : Command(THROW),
    projectile(projectile_in),
    target(target_in)
{
}

void CmdThrow::run(GameState* g)
{
    if(projectile)
    {
        Object* player = g->world->get_player();
        if(projectile->parent == player)
        {
            if(target)
            {
                g->engine->terminal->disp("You throw the " + projectile->pretty_name + " at the " + target->pretty_name + ".");
//                if(target->has_component(Component::BREAKABLE))
                    //g->send_front(std::make_shared<CmdBreak>(target));
            }
            else
                g->engine->terminal->disp("You throw the " + projectile->pretty_name + ".");
            player->remove_child(projectile);
            g->world->get_current_room()->add_child(projectile);
        }
        else
            g->engine->terminal->disp("You're not carrying the " + projectile->pretty_name + ".");
    }
}


