#include "Action.h"
#include "Player.h"
#include "Engine.h"
#include "Event.h"
#include "Room.h"
#include "Word.h"
#include "WordList.h"
#include "World.h"
#include "Receiver.h"
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

void Action::run(World* w, Receiver* r)
{
    bool run = true;
    Room* cur_room = w->get_current_room();
    if(cur_room)
        run = cur_room->pre_action(w, r, this, cur_room);
    if(run && objects.empty())
        act(w, r, NULL);
    else if(!objects.empty())
    {
        for(int i = 0; i < objects.size(); i++)
        {
            if(objects[i]->pre_action(w, r, this, objects[i]))
                act(w, r, objects[i]);
            objects[i]->post_action(w, r, this, objects[i]);
        }
    }
    if(cur_room)
        cur_room->post_action(w, r, this, cur_room);

}

void Action::act(World* w, Receiver* r, Object* o)
{

}

void ActionGo::act(World* w, Receiver* r, Object* o)
{
    if(o)
    {
        if(o->properties & Object::GOABLE)
        {
			std::string new_room_name = o->goable_data;
			w->set_current_room(new_room_name, r);
        }
        else
        {
			r->add_event(new CmdDisp("You can't go there, baka."));
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
            r->add_event(new CmdDisp("Go where?"));
        }
        else
        {
            Room* room = w->get_current_room();
            if(room && room->directions[direction] != "") {
                w->set_current_room(room->directions[direction], r);
            } else {
                r->add_event(new CmdDisp("You can't go " + dir[direction].name + " from here."));
            }
        }
    }
}

void ActionLook::act(World* w, Receiver* r, Object* o)
{
    if(o)
        o->describe(r, true, true);
    else
    {
        Room* room = w->get_current_room();
        if(room)
            room->describe(r, true, true);
    }
}

void ActionQuit::act(World* w, Receiver* r, Object* o)
{
	w->active = false;
}

void ActionTake::act(World* w, Receiver* r, Object* o)
{
	if (o && (o->properties & Object::TAKEABLE))
	{
		r->add_event(new CmdDisp("You take the " + o->name.word + "."));
		if (o->parent)
			o->parent->remove_child(o);
		w->player->add_child(o);
	}
	else if (o)
	{
		r->add_event(new CmdDisp("You can't take the " + o->name.word + ", baka gaijin!"));
	}
	else
	{
		r->add_event(new CmdDisp("Take what?"));
	}
}

void ActionWear::act(World* w, Receiver* r, Object* o)
{
	if (o && (o->properties & Object::WEARABLE))
	{
		r->add_event(new CmdDisp("You put on the " + o->name.word + "."));
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
		r->add_event(new CmdDisp("You can't wear a " + o->name.word + "."));
	}
	else
	{
		r->add_event(new CmdDisp("Wear what?"));
	}
}

void ActionHit::act(World* w, Receiver* r, Object* o)
{
	if (o && (o->properties & Object::HITTABLE))
	{
		if (o->flipped)
			r->add_event(new CmdDisp("The " + o->name.word + " has already been hit."));
		else
		{
			r->add_event(new CmdDisp("You hit the " + o->name.word + "."));
			o->flipped = true;
		}
	}
	else if (o)
	{
		r->add_event(new CmdDisp("You can't hit the " + o->name.word));
	}
	else
	{
		r->add_event(new CmdDisp("Hit what?"));
	}
}

void ActionOpenContainer::act(World* w, Receiver* r, Object* o)
{
	if (o && o->properties & Object::CONTAINER)
	{
		if (!o->open)
		{
			r->add_event(new CmdDisp("You open the " + o->name.word + "."));
			o->open = true;
			o->describe(r, true, false);
		}
		else
		{
			r->add_event(new CmdDisp("The " + o->name.word + " is already open."));
		}
	}
	else if(o)
	{
		r->add_event(new CmdDisp("You can't open a " + o->name.word + "!"));
	}
	else
	{
		r->add_event(new CmdDisp("Open what?"));
	}
}

void ActionShout::act(World* w, Receiver* r, Object* o)
{
	r->add_event(new CmdDisp("SHEEEEIT!"));
}

void ActionRead::act(World* w, Receiver* r, Object* o)
{
    if(o && (o->properties & Object::READABLE))
    {
        r->add_event(new CmdDisp("The " + o->name.word + " reads:"));
        r->add_event(new CmdDisp(o->readable_data));
    }
    else if(o)
    {
        r->add_event(new CmdDisp("There's nothing to read on the " + o->name.word + "."));
    }
    else
    {
        r->add_event(new CmdDisp("Read what?"));
    }
}

void ActionTalkTo::act(World* w, Receiver* r, Object* o)
{
    if(o && (o->properties & Object::TALKABLE))
    {
        for(int i = 0; i < o->talkable_data.size(); i++)
        {
            r->add_event(new CmdDisp(o->talkable_data[i]));
            r->add_event(new CmdPause());
        }
    }
    else if(o)
    {
        r->add_event(new CmdDisp("You can't talk to the " + o->name.word + ", baka gaijin!"));
    }
    else
    {
        r->add_event(new CmdDisp("Talk to what?"));
    }
}

void ActionHelp::act(World* w, Receiver* r, Object* o)
{
    //e->terminal->set_color(config::colors[config::color_objective]);
    //e->disp("Your objective: " + w->player->objective);
    /*e->disp("It's little Kodak, the finesse kid, boy who hot as me?\nTold the doctor I'm a healthy kid, I smoke broccoli");
    e->disp("Type sentences to do stuff. You can use the following verbs:");
    
    for(int i = 0; i < name.parent_list->ACTION_MAX; i++)
    {
        e->disp("-" + name.parent_list->words_by_id[i].word);
    }*/
    //e->terminal->set_color();
}
