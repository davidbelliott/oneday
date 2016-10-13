#include "Action.h"
#include "Terminal.h"
#include "Room.h"
#include "Word.h"
#include "WordList.h"
#include "World.h"
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

void Action::run(World* w, Terminal* t)
{
    bool run = true;
    Room* cur_room = w->get_current_room();
    if(cur_room)
        run = cur_room->pre_action(w, t, this, cur_room);
    if(run && objects.empty())
        act(w, t, NULL);
    else if(!objects.empty())
    {
        for(int i = 0; i < objects.size(); i++)
        {
            if(objects[i]->pre_action(w, t, this, objects[i]))
                act(w, t, objects[i]);
            objects[i]->post_action(w, t, this, objects[i]);
        }
    }
    if(cur_room)
        cur_room->post_action(w, t, this, cur_room);

}

void Action::act(World* w, Terminal* t, Object* o)
{

}

void ActionGo::act(World* w, Terminal* t, Object* o)
{
    if(o)
    {
        if(o->properties & Object::GOABLE)
        {
			std::string new_room_name = o->goable_data;
			w->set_current_room(new_room_name, t);
        }
        else
        {
			t->disp("You can't go there, baka.");
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
            t->disp("Go where?");
        }
        else
        {
            Room* room = w->get_current_room();
            if(room && room->directions[direction] != "") {
                w->set_current_room(room->directions[direction], t);
            } else {
                t->disp("You can't go " + dir[direction].name + " from here.");
            }
        }
    }
}

void ActionLook::act(World* w, Terminal* t, Object* o)
{
    if(o)
        o->describe(t, true, true);
    else
    {
        Room* room = w->get_current_room();
        if(room)
            room->describe(t, true, true);
    }
}

void ActionQuit::act(World* w, Terminal* t, Object* o)
{
	w->active = false;
}

void ActionTake::act(World* w, Terminal* t, Object* o)
{
	if (o && (o->properties & Object::TAKEABLE))
	{
		t->disp("You take the " + o->name.word + ".");
		w->inventory.push_back(o);
		if (o->parent)
			o->parent->remove_child(o);
	}
	else if (o)
	{
		t->disp("You can't take the " + o->name.word + ", baka gaijin!");
	}
	else
	{
		t->disp("Take what?");
	}
}

void ActionWear::act(World* w, Terminal* t, Object* o)
{
	if (o && (o->properties & Object::WEARABLE))
	{
		t->disp("You put on the " + o->name.word + ".");
		w->clothing = o;
		if (o->parent)
			o->parent->remove_child(o);
	}
	else if (o)
	{
		t->disp("You can't wear a " + o->name.word + ".");
	}
	else
	{
		t->disp("Wear what?");
	}
}

void ActionHit::act(World* w, Terminal* t, Object* o)
{
	if (o && (o->properties & Object::HITTABLE))
	{
		if (o->flipped)
			t->disp("The " + o->name.word + " has already been hit.");
		else
		{
			t->disp("You hit the " + o->name.word + ".");
			o->flipped = true;
		}
	}
	else if (o)
	{
		t->disp("You can't hit the " + o->name.word);
	}
	else
	{
		t->disp("Hit what?");
	}
}

void ActionOpenContainer::act(World* w, Terminal* t, Object* o)
{
	if (o && o->properties & Object::CONTAINER)
	{
		if (!o->open)
		{
			t->disp("You open the " + o->name.word + ".");
			o->open = true;
			o->describe(t, true, false);
		}
		else
		{
			t->disp("The " + o->name.word + " is already open.");
		}
	}
	else if(o)
	{
		t->disp("You can't open a " + o->name.word + "!");
	}
	else
	{
		t->disp("Open what?");
	}
}

void ActionShout::act(World* w, Terminal* t, Object* o)
{
	t->disp("SHEEEEIT!");
}

void ActionRead::act(World* w, Terminal* t, Object* o)
{
    if(o && (o->properties & Object::READABLE))
    {
        t->disp("The " + o->name.word + " reads:");
        t->disp(o->readable_data);
    }
    else if(o)
    {
        t->disp("There's nothing to read on the " + o->name.word + ".");
    }
    else
    {
        t->disp("Read what?");
    }
}

void ActionTalkTo::act(World* w, Terminal* t, Object* o)
{
    if(o && (o->properties & Object::TALKABLE))
    {
        for(int i = 0; i < o->talkable_data.size(); i++)
        {
            t->disp(o->talkable_data[i]);
            t->pause();
        }
    }
    else if(o)
    {
        t->disp("You can't talk to the " + o->name.word + ", baka gaijin!");
    }
    else
    {
        t->disp("Talk to what?");
    }
}

void ActionHelp::act(World* w, Terminal* t, Object* o)
{
    t->set_color(sf::Color::Red);
    t->disp("It's little Kodak, the finesse kid, boy who hot as me?\nTold the doctor I'm a healthy kid, I smoke broccoli");
    t->disp("Type sentences to do stuff. You can use the following verbs:");
    
    for(int i = 0; i < name.parent_list->ACTION_MAX; i++)
    {
        t->disp("-" + name.parent_list->words_by_id[i].word);
    }
    t->set_color();
}
