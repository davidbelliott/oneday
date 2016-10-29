#include "Action.h"
#include "Player.h"
#include "Engine.h"
#include "Room.h"
#include "Word.h"
#include "WordList.h"
#include "World.h"
#include "Terminal.h"
#include "common.h"

Action::Action(Word name_in)
: name(name_in)
{
}

Action::~Action()
{
}

void Action::add_object(Object* object_in)
{
	objects.push_back(object_in);
}

void Action::add_preposition(Word preposition_in)
{
	prepositions.push_back(preposition_in);
}

void Action::run(World* w, Engine* e)
{
    bool run = true;
    Room* cur_room = w->get_current_room();
    if(cur_room)
        run = cur_room->pre_action(w, e->terminal, this, cur_room);
    if(run && objects.empty())
        act(w, e, NULL);
    else if(!objects.empty())
    {
        for(int i = 0; i < objects.size(); i++)
        {
            if(objects[i]->pre_action(w, e->terminal, this, objects[i]))
                act(w, e, objects[i]);
            objects[i]->post_action(w, e->terminal, this, objects[i]);
        }
    }
    if(cur_room)
        cur_room->post_action(w, e->terminal, this, cur_room);

}

void Action::act(World* w, Engine* e, Object* o)
{

}

void ActionGo::act(World* w, Engine* e, Object* o)
{
    if(o)
    {
        if(o->properties & Object::GOABLE)
        {
			std::string new_room_name = o->goable_data;
			w->set_current_room(new_room_name, e);
        }
        else
        {
			e->disp("You can't go there, baka.");
        }

    }
    else
    {
        DirectionId direction = DIRECTION_MAX;
        for(int i = 0; i < prepositions.size() && direction == DIRECTION_MAX; i++)
        {
            for(int j = 0; j < DIRECTION_MAX && direction == DIRECTION_MAX; j++)
            {
                if(prepositions[i].word == dir[(DirectionId)j].name)
                    direction = (DirectionId)j;
            }
        }
        if(direction == DIRECTION_MAX)
        {
            e->disp("Go where?");
        }
        else
        {
            Room* room = w->get_current_room();
            if(room && room->directions[direction] != "") {
                w->set_current_room(room->directions[direction], e);
            } else {
                e->disp("You can't go " + dir[direction].name + " from here.");
            }
        }
    }
}

void ActionLook::act(World* w, Engine* e, Object* o)
{
    if(o)
        o->describe(e->terminal, true, true);
    else
    {
        Room* room = w->get_current_room();
        if(room)
            room->describe(e->terminal, true, true);
    }
}

void ActionQuit::act(World* w, Engine* e, Object* o)
{
	w->active = false;
}

void ActionTake::act(World* w, Engine* e, Object* o)
{
	if (o && (o->properties & Object::TAKEABLE))
	{
		e->disp("You take the " + o->name.word + ".");
		if (o->parent)
			o->parent->remove_child(o);
		w->player->add_child(o);
	}
	else if (o)
	{
		e->disp("You can't take the " + o->name.word + ", baka gaijin!");
	}
	else
	{
		e->disp("Take what?");
	}
}

void ActionWear::act(World* w, Engine* e, Object* o)
{
	if (o && (o->properties & Object::WEARABLE))
	{
		e->disp("You put on the " + o->name.word + ".");
        if(o->parent)
        {
            if(o->parent != w->player)
            {
                o->parent->remove_child(o);
                w->player->add_child(o);
            }
        }
        else
        {
            w->player->add_child(o);
        }
		w->player->clothing = o->name.word;
	}
	else if (o)
	{
		e->disp("You can't wear a " + o->name.word + ".");
	}
	else
	{
		e->disp("Wear what?");
	}
}

void ActionHit::act(World* w, Engine* e, Object* o)
{
	if (o && (o->properties & Object::HITTABLE))
	{
		if (o->flipped)
			e->disp("The " + o->name.word + " has already been hit.");
		else
		{
			e->disp("You hit the " + o->name.word + ".");
			o->flipped = true;
		}
	}
	else if (o)
	{
		e->disp("You can't hit the " + o->name.word);
	}
	else
	{
		e->disp("Hit what?");
	}
}

void ActionOpenContainer::act(World* w, Engine* e, Object* o)
{
	if (o && o->properties & Object::CONTAINER)
	{
		if (!o->open)
		{
			e->disp("You open the " + o->name.word + ".");
			o->open = true;
			o->describe(e->terminal, true, false);
		}
		else
		{
			e->disp("The " + o->name.word + " is already open.");
		}
	}
	else if(o)
	{
		e->disp("You can't open a " + o->name.word + "!");
	}
	else
	{
		e->disp("Open what?");
	}
}

void ActionShout::act(World* w, Engine* e, Object* o)
{
	e->disp("SHEEEEIT!");
}

void ActionRead::act(World* w, Engine* e, Object* o)
{
    if(o && (o->properties & Object::READABLE))
    {
        e->disp("The " + o->name.word + " reads:");
        e->disp(o->readable_data);
    }
    else if(o)
    {
        e->disp("There's nothing to read on the " + o->name.word + ".");
    }
    else
    {
        e->disp("Read what?");
    }
}

void ActionTalkTo::act(World* w, Engine* e, Object* o)
{
    if(o && (o->properties & Object::TALKABLE))
    {
        for(int i = 0; i < o->talkable_data.size(); i++)
        {
            e->disp(o->talkable_data[i]);
            e->terminal->pause();
        }
    }
    else if(o)
    {
        e->disp("You can't talk to the " + o->name.word + ", baka gaijin!");
    }
    else
    {
        e->disp("Talk to what?");
    }
}

void ActionHelp::act(World* w, Engine* e, Object* o)
{
    e->terminal->set_color(config::colors[config::color_objective]);
    e->disp("Your objective: " + w->player->objective);
    /*e->disp("It's little Kodak, the finesse kid, boy who hot as me?\nTold the doctor I'm a healthy kid, I smoke broccoli");
    e->disp("Type sentences to do stuff. You can use the following verbs:");
    
    for(int i = 0; i < name.parent_list->ACTION_MAX; i++)
    {
        e->disp("-" + name.parent_list->words_by_id[i].word);
    }*/
    e->terminal->set_color();
}
