#include "Command.h"
#include "GameState.h"
#include "Engine.h"
#include "Terminal.h"
#include "World.h"
#include "Directions.h"
#include "Player.h"
#include <set>

Command::Command(CommandType type_in)
    : type(type_in),
      objects({})
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

void Command::run_with_callbacks(GameState* g)
{
    bool run = true;
    for(int i = 0; i < objects.size() && run; i++)
        run = objects[i]->before(this);
    if(run)
    {
        this->run(g);
        for(int i = 0; i < objects.size(); i++)
            objects[i]->after(this);
    }
}

CmdGo::CmdGo()
    : Command(GO),
      new_room("")
{

}

bool CmdGo::try_to_go(DirectionId direction, std::vector<std::string>* errors, World* world)
{
    bool can_go = true;
    if(Object* room = world->get_current_room())
    {
        ComponentRoom* room_component = (ComponentRoom*)room->get_component(Component::ROOM);
        if(room_component && room_component->directions[direction] != "")
            new_room = room_component->directions[direction];
        else
            can_go = false;
    }
    else
        can_go = false;
    if(!can_go)
        errors->push_back("You can't go " + dir[direction].name + " from here, baka!");
    return can_go;
}

bool CmdGo::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args = {};

    if(matches(str, "go in #", args) || 
       matches(str, "go out #", args) ||
       matches(str, "go through #", args) ||
       matches(str, "go into #", args) ||
       matches(str, "jump into #", args) ||
       matches(str, "enter #", args))
    {
        Object* o = get_object(args[0], world);
        if(o)
        {
            ComponentPortal* c_port = (ComponentPortal*)o->get_component(Component::PORTAL);
            if(c_port)
            {
                ComponentOpenClose* c_open_close = (ComponentOpenClose*)o->get_component(Component::OPEN_CLOSE);
                if(c_open_close && !c_open_close->open)
                {
                    errors->push_back("The " + o->pretty_name + " is closed.");
                }
                else
                {
                    new_room = c_port->destination;
                    match = true;
                }
            }
            else
            {
                errors->push_back("You can't go into the " + o->pretty_name + ".");
            }
        }
        else
        {
            errors->push_back("You can find no " + join(args[0], ' ') + " to go in.");
        }
    }

    if(!match &&
            (  matches(str, "go north", args)
            || matches(str, "go n", args)
            || matches(str, "north", args)
            || matches(str, "n", args)))
    {
        match = try_to_go(NORTH, errors, world);
    }

    if(!match &&
            ( matches(str, "go east", args)
            || matches(str, "go e", args)
            || matches(str, "east", args)
            || matches(str, "e", args)))

    {
        match = try_to_go(EAST, errors, world);
    }

    if(!match &&
            ( matches(str, "go south", args)
            || matches(str, "go s", args)
            || matches(str, "south", args)
            || matches(str, "s", args)))
    {
        match = try_to_go(SOUTH, errors, world);
    }

    if(!match &&
            ( matches(str, "go west", args)
            || matches(str, "go w", args)
            || matches(str, "west", args)
            || matches(str, "w", args)))
    {
        match = try_to_go(WEST, errors, world);
    }

    if(!match && 
            ( matches(str, "go up", args)
            || matches(str, "go u", args)
            || matches(str, "climb up", args)
            || matches(str, "climb", args)
            || matches(str, "climb #", args)
            || matches(str, "up", args)
            || matches(str, "u", args)))
    {
        match = try_to_go(UP, errors, world);
    }

    if(!match &&
            ( matches(str, "go down", args)
            || matches(str, "go d", args)
            || matches(str, "climb down", args)
            || matches(str, "climb down #", args)
            || matches(str, "down", args)
            || matches(str, "d", args)))
    {
        match = try_to_go(DOWN, errors, world);
    }

    if(!match && matches(str, "go #", args))
    {
        errors->push_back(join(args[0], ' ') + " isn't a valid direction.");
    }

    return match;
}

void CmdGo::run(GameState* g)
{
    Object* rm = nullptr;
    if((rm = g->world->get_indirect_child(new_room, 0)) && rm->before(this))
    {
        ComponentMusic* music_leaving = (ComponentMusic*)g->world->get_current_room()->get_component(Component::MUSIC);
        g->world->set_current_room(new_room);
        ComponentMusic* music_entering = (ComponentMusic*)rm->get_component(Component::MUSIC);
        if(music_leaving)
            g->engine->audio->pause_music(music_leaving->music);
        if(music_entering)
            g->engine->audio->play_music(music_entering->music, music_entering->start_time);

        CmdLookAround look_around;
        look_around.run(g);
        rm->after(this);
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

bool CmdQuit::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;
    if(matches(str, "quit", args)
        || matches(str, "q", args)
        || matches(str, "exit", args)
        || matches(str, "bye bye", args))
    {
        /*GameStateMenu* menu = new GameStateMenu(
                        g->engine,
                        g,
                        "Are you sure you want to quit? (y/n)",
                        {{"y", {new CmdQuit()}},
                         {"n", {}}});
        commands.push_back(new CmdAddGameState(menu));*/
        match = true;
    }
    return match;
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

bool CmdLookAround::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;
    if(matches(str, "look around", args)
            || matches(str, "look", args)
            || matches(str, "l", args)
            || matches(str, "x", args))
    {
        match = true;
    }
    return match;
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

bool CmdExamine::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;
    if(matches(str, "examine #", args)
        || matches(str, "inspect #", args)
        || matches(str, "look at #", args)
        || matches(str, "look #", args)
        || matches(str, "l #", args)
        || matches(str, "x #", args))
    {
        Object* obj = get_object(args[0], world);
        if(obj)
        {
            objects = { obj };
            match = true;
        }
        else
            errors->push_back("There is no " + join(args[0], ' ') + " for you to examine here.");
    }
    return match;
}

void CmdExamine::run(GameState* g)
{
    if(objects.size() == 1)
        recursive_show(g, objects[0], true, false, true);
}

CmdTake::CmdTake()
    : Command(TAKE)
{ }

bool CmdTake::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;
    if(matches(str, "take #", args)
            || matches(str, "get #", args)
            || matches(str, "pick up #", args)
            || matches(str, "pick # up", args)
            || matches(str, "grab #", args)
            || matches(str, "snatch #", args)
            || matches(str, "obtain #", args)
            || matches(str, "grasp #", args))
    {
        Object* obj = get_object(args[0], world);
        if(!obj)
            errors->push_back("There's no " + join(args[0], ' ') + " for you to take here.");
        else
        {
            if(obj->has_component(Component::TAKEABLE))
            {
                ComponentTie* c_tie = (ComponentTie*)obj->get_component(Component::TIE);
                if(c_tie && c_tie->tie_to.size() > 0)
                    errors->push_back("The " + obj->pretty_name + " is tied to the " + c_tie->tie_to[0]->pretty_name + ".");
                else
                {
                    objects = { obj };
                    match = true;
                }
            }
            else
                errors->push_back("You can't pick up the " + obj->pretty_name + ".");
        }
    }
    return match;
}

void CmdTake::run(GameState* g)
{
    if(objects.size() == 1)
    {
        Object* object = objects[0];
        if(object->parent == g->world->get_player())
            g->engine->terminal->disp("You already have the " + object->pretty_name + ".");
        else
        {
            if(object->parent)
                object->parent->remove_child(object);
            if(g->world->get_player())
                g->world->get_player()->add_child(object);
            g->engine->terminal->disp("You take the " + object->pretty_name + ".");
        }
    }
}

CmdHit::CmdHit()
    : Command(HIT)
{ }

bool CmdHit::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;

    if(matches(str, "hit #", args)
            || matches(str, "flip #", args)
            || matches(str, "toggle #", args)
            || matches(str, "pull #", args)
            || matches(str, "push #", args)
            || matches(str, "yank #", args)
            || matches(str, "depress #", args))
    {
        Object* obj = get_object(args[0], world);
        if(obj)
        {
            ComponentHittable* c_hittable = (ComponentHittable*)obj->get_component(Component::HITTABLE);
            if(c_hittable)
            {
                this->objects = { obj };
                match = true;
            }
            else
                errors->push_back("You can't hit the " + join(args[0], ' ') + ", baka!");
        }
        else
            errors->push_back("There's no " + join(args[0], ' ') + " for you to hit here.");
    }
    return match;
}

void CmdHit::run(GameState* g)
{
    if(objects.size() == 1)
    {
        ComponentHittable* c_hittable = (ComponentHittable*)objects[0]->get_component(Component::HITTABLE);
        if(c_hittable)
            c_hittable->flipped = !c_hittable->flipped;
    }
}

CmdWear::CmdWear()
    : Command(WEAR)
{ }

bool CmdWear::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;

    if(matches(str, "wear #", args)
            || matches(str, "put on #", args)
            || matches(str, "put # on", args)
            || matches(str, "don #", args)
            || matches(str, "dress in #", args))
    {

        Object* obj = get_object(args[0], world);
        if(obj)
        {
            if(obj->has_component(Component::WEARABLE))
            {
                this->objects = { obj };
                match = true;
            }
            else
                errors->push_back("You can't wear a " + obj->pretty_name + ", silly~");
        }
        else
            errors->push_back("You see no such thing.");
    }
    return match;
}

void CmdWear::run(GameState* g)
{
    if(objects.size() == 1)
    {
        if(objects[0]->parent)
            objects[0]->parent->remove_child(objects[0]);
        if(g->world->get_player())
        {
            g->world->get_player()->add_child(objects[0]);
            ((Player*)g->world->get_player())->clothing = objects[0]->name;
        }
        g->engine->terminal->disp("You put on the " + objects[0]->pretty_name + ".");
    }
}

CmdRead::CmdRead()
    : Command(READ)
{ }

bool CmdRead::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;
    if(matches(str, "read #", args)
        || matches(str, "peruse #", args)
        || matches(str, "browse #", args))
    {
        Object* obj = get_object(args[0], world);
        if(obj)
        {
            objects = { obj };
            match = true;
        }
        else
            errors->push_back("There's no " + join(args[0], ' ') + "for you to read.");
    }
    return match;
}

void CmdRead::run(GameState* g)
{
    if(objects.size() == 1)
    {
        Object* object = objects[0];
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
}

CmdMove::CmdMove()
    : Command(MOVE)
{ }

bool CmdMove::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;
    if(matches(str, "move #", args))
    {
        Object* obj = get_object(args[0], world);
        if(obj)
        {
            objects = { obj };
            match = true;
        }
        else
            errors->push_back("You can find no " + join(args[0], ' ') + " to move here.");
    }
    return match;
}

void CmdMove::run(GameState* g)
{
    if(objects.size() == 1)
    {
        ComponentMoveable* c_move = (ComponentMoveable*)objects[0]->get_component(Component::MOVEABLE);
        ComponentDescription* c_desc = (ComponentDescription*)objects[0]->get_component(Component::DESCRIPTION);
        if(c_move)
        {
            if(c_move->new_parent)
            {
                objects[0]->parent->remove_child(objects[0]);
                c_move->new_parent->add_child(objects[0]);
            }
            if(c_desc)
                c_desc->current_appearance = "There is a " + objects[0]->pretty_name + " here.";
            g->engine->terminal->disp("With considerable effort, you move the " + objects[0]->pretty_name + " aside.");
        }
        else
        {
            g->engine->terminal->disp("Try as you might, the " + objects[0]->pretty_name + " will not budge.");
        }
    }
}

CmdTalkTo::CmdTalkTo()
    : Command(TALK_TO)
{ }

bool CmdTalkTo::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;
    if(matches(str, "talk to #", args)
            || matches(str, "talk with #", args)
            || matches(str, "converse with #", args))
    {

        Object* obj = get_object(args[0], world);
        if(obj)
        {
            //talk_to->object = obj;
            match = true;
        }
        else
            errors->push_back("You can't find a " + join(args[0], ' ') + " to talk to.");
    }
    return match;
}

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

CmdEat::CmdEat()
    : Command(EAT)
{
}

bool CmdEat::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;
    if(matches(str, "eat #", args))
    {
        Object* obj = get_object(args[0], world);
        if(obj)
        {
            if(obj->has_component(Component::EDIBLE))
            {
                objects = {obj};
                match = true;
            }
            else
                errors->push_back("The " + obj->pretty_name + " is not edible.");
        }
        else
            errors->push_back("There is no " + join(args[0], ' ') + " here.");
    }
    return match;
}

void CmdEat::run(GameState* g)
{
    if(objects.size() == 1)
    {
        g->engine->terminal->disp("You eat the " + objects[0]->pretty_name + " and find it most satisfactory.");
        if(objects[0]->parent)
            objects[0]->parent->remove_child(objects[0]);
        objects[0]->active = false;
    }
}

CmdGive::CmdGive()
    : Command(GIVE)
{

}

bool CmdGive::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;
    if(matches(str, "give # to #", args)
        || matches(str, "feed # to #", args))
    {
        Object* obj = get_object(args[0], world);
        Object* actor = get_object(args[1], world);
        if(actor)
        {
            if(actor->has_component(Component::TALKABLE))
            {
                if(obj)
                {
                    if(obj->has_component(Component::TAKEABLE))
                    {
                        match = true;
                        objects = { actor, obj };
                    }
                    else
                        errors->push_back("The " + obj->pretty_name + " can't be taken.");
                }
                else
                    errors->push_back("There is no " + join(args[0], ' ') + " here.");
            }
            else
                errors->push_back(">He thinks he can give something to a " + actor->pretty_name);
        }
        else
            errors->push_back("There is no " + join(args[1], ' ') + "here.");
    }
    return match;
}

void CmdGive::run(GameState* g)
{
    if(objects.size() == 2)
    {
        g->engine->terminal->disp("You give the " + objects[1]->pretty_name + " to " + objects[0]->pretty_name + ".");
        if(objects[1]->parent)
            objects[1]->parent->remove_child(objects[1]);
        objects[1]->active = false;
    }
}

CmdOpen::CmdOpen()
    : Command(OPEN)
{

}

bool CmdOpen::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;
    if(matches(str, "open #", args))
    {
        Object* obj = get_object(args[0], world);
        if(obj)
        {
            ComponentOpenClose* c_open = (ComponentOpenClose*)obj->get_component(Component::OPEN_CLOSE);
            if(c_open)
            {
                if(!c_open->open)
                {
                    match = true;
                    objects = { obj };
                }
                else
                    errors->push_back("It's already open.");
            }
            else
                errors->push_back("It can't be opened.");
        }
        else
            errors->push_back("You see no such thing.");
    }
    return match;
}

void CmdOpen::run(GameState* g)
{
    if(objects.size() == 1)
    {
        ComponentOpenClose* c_open = (ComponentOpenClose*)objects[0]->get_component(Component::OPEN_CLOSE);
        c_open->open = true;
        g->engine->terminal->disp("You open the " + objects[0]->pretty_name + ".");
        recursive_show(g, objects[1], true, false, false);
    }
}

CmdClose::CmdClose()
    : Command(CLOSE)
{
}

bool CmdClose::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;
    if(matches(str, "close #", args))
    {
        Object* obj = get_object(args[0], world);
        if(obj)
        {
            ComponentOpenClose* c_open = (ComponentOpenClose*)obj->get_component(Component::OPEN_CLOSE);
            if(c_open)
            {
                if(c_open->open)
                {
                    match = true;
                    objects = {obj};
                }
                else
                    errors->push_back("It's already closed.");
            }
            else
                errors->push_back("It can't be closed.");
        }
        else
            errors->push_back("You see no such thing.");
    }
    return match;
}

void CmdClose::run(GameState* g)
{
    if(objects.size() == 1)
    {
       ComponentOpenClose* c_open_close = (ComponentOpenClose*)objects[0]->get_component(Component::OPEN_CLOSE);
       
        if(!c_open_close)
            g->engine->terminal->disp("You can't close the " + objects[0]->pretty_name + "!");
        else if(!c_open_close->open)
            g->engine->terminal->disp("The " + objects[0]->pretty_name + " is already closed.");
        else
        {
            c_open_close->open = false;
            g->engine->terminal->disp("You close the " + objects[0]->pretty_name + ".");
            //recursive_show(g, obj, true, false, false);
        }
    }
}

CmdTieTo::CmdTieTo()
    : Command(TIE_TO)
{
}

bool CmdTieTo::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;
    if(matches(str, "tie # to #", args)
            || matches(str, "tie # around #", args)
            || matches(str, "attach # to #", args)
            || matches(str, "fasten # to #", args)
            || matches(str, "mount # to #", args))
    {
        Object* tie = get_object(args[0], world);
        Object* tie_to = get_object(args[1], world);
        if(tie_to)
        {
            if(tie_to->has_component(Component::TIE_TO))
            {
                if(tie)
                {
                    if(tie->has_component(Component::TIE))
                    {
                        match = true;
                        objects = {tie, tie_to};
                    }
                    else
                        errors->push_back("The " + tie->pretty_name + " can't be tied.");
                }
                else
                    errors->push_back("There's no " + join(args[0], ' ') + " for you to tie.");
            }
            else
                errors->push_back("Nothing can be tied to the " + tie_to->pretty_name + ".");
        }
        else
            errors->push_back("There's no " + join(args[1], ' ') + " for you to tie something to.");
    }
    return match;
}

void CmdTieTo::run(GameState* g)
{
    if(objects.size() == 2)
    {
        Object* tie = objects[0];
        Object* tie_to = objects[1];
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

CmdInv::CmdInv()
    : Command(INVENTORY)
{
}

bool CmdInv::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;
    if(matches(str, "i", args)
            || matches(str, "inv", args)
            || matches(str, "inventory", args))
    {
        Object* player = world->get_player();
        if(player)
        {
            if(player->has_component(Component::INVENTORY))
            {
                match = true;
                objects = { player };
            }
            else
                errors->push_back(player->pretty_name + " has no inventory.");
        }
        else
            errors->push_back("There is no player.");
    }
    return match;
}

void CmdInv::run(GameState* g)
{
    if(objects.size() == 1)
    {
        Object* player = objects[0];
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

CmdThrow::CmdThrow()
    : Command(THROW)
{
}

bool CmdThrow::match(std::string str, std::vector<std::string>* errors, World* world)
{
    bool match = false;
    arg_list args;
    if(matches(str, "throw # at #", args)
            || matches(str, "drop # on #", args))
    {
        Object* player = world->get_player();
        Object* projectile = get_object(args[0], world);
        Object* target = get_object(args[1], world);
        if(projectile)
        {
            if(projectile->parent == player)
            {
                if(target)
                {
                    match = true;
                    objects = {projectile, target};
                }
                else
                    errors->push_back("There ain't no " + join(args[1], ' ') + " to throw shit at, nigga.");
            }
            else
                errors->push_back("You ain't carrying the " + projectile->pretty_name + ".");
        }
        else
            errors->push_back("You have no " + join(args[0], ' ') + " to throw.");
    }
    else if(matches(str, "throw #", args)
            || matches(str, "drop #", args))
    {
        Object* player = world->get_player();
        Object* projectile = get_object(args[0], world);
        if(projectile)
        {
            if(projectile->parent == player)
            {
                objects = { projectile };
                match = true;
            }
            else
                errors->push_back("You ain't carrying the " + projectile->pretty_name + ".");
        }
        else
            errors->push_back("You have no " + join(args[0], ' ') + " to throw.");
    }
    return match;
}

void CmdThrow::run(GameState* g)
{
    if(objects.size() >= 1)
    {
        Object* player = g->world->get_player();
        Object* projectile = objects[0];
        if(objects.size() == 2)
        {
            Object* target = objects[1];
            g->engine->terminal->disp("You throw the " + projectile->pretty_name + " at the " + target->pretty_name + ".");
//                if(target->has_component(Component::BREAKABLE))
                //g->send_front(std::make_shared<CmdBreak>(target));
        }
        else
            g->engine->terminal->disp("You throw the " + projectile->pretty_name + ".");
        player->remove_child(projectile);
        g->world->get_current_room()->add_child(projectile);
    }
}


