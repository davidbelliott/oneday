
#include "Action.h"
#include "Terminal.h"
#include "Room.h"
#include "Word.h"
#include "World.h"
#include "common.h"

Action::Action(Word word_in)
: word(word_in)
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
	if (objects.empty())
		act(w, t, NULL);
	else
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i]->run_action(w, t, this, objects[i]))
				act(w, t, objects[i]);
		}
	}
}

void Action::act(World* w, Terminal* t, Object* o)
{

}

void ActionGo::act(World* w, Terminal* t, Object* o)
{
	if (!o && prepositions.empty())
	{
		t->disp("Go where?");
	}
	else if(!prepositions.empty())
	{
		std::string modifier_name = prepositions[0].get_name();
		Room* room = (Room*)w->get_direct_child(w->cur_room);

		int direction = -1;
		if (modifier_name == "north")
			direction = NORTH;
		else if (modifier_name == "east")
			direction = EAST;
		else if (modifier_name == "south")
			direction = SOUTH;
		else if (modifier_name == "west")
			direction = WEST;
		else if (modifier_name == "up")
			direction = UP;
		else if (modifier_name == "down")
			direction = DOWN;
		if (direction != -1)
		{
			if (room->directions[direction] != "")
				w->set_current_room(room->directions[direction], t);
			else
				t->disp("You can't go " + modifier_name + " from here.");
		}
	}
	if(o)
	{
		if (o->properties & Object::GOABLE)
		{
			std::string new_room_name = o->goable_data;
			w->set_current_room(new_room_name, t);
		}
		else
		{
			t->disp("You can't go through the " + o->get_name() + ", baka.");
		}
	}
}

void ActionLook::act(World* w, Terminal* t, Object* o)
{
	if (!o)
	{
		Room* room = w->get_current_room();
		if (room->run_action(w, t, this, room))
			room->describe(t, true, true);
	}
	else
	{
		o->describe(t, true, true);
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
		t->disp("You take the " + o->get_name() + ".");
		w->inventory.push_back(o);
		if (o->parent)
			o->parent->remove_child(o);
	}
	else if (o)
	{
		t->disp("You can't take the " + o->get_name() + ", baka gaijin!");
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
		t->disp("You put on the " + o->get_name() + ".");
		w->clothing = o;
		if (o->parent)
			o->parent->remove_child(o);
	}
	else if (o)
	{
		t->disp("You can't wear a " + o->get_name() + ".");
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
			t->disp("The " + o->get_name() + " has already been hit.");
		else
		{
			t->disp("You hit the " + o->get_name() + ".");
			o->flipped = true;
		}
	}
	else if (o)
	{
		t->disp("You can't hit the " + o->get_name());
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
			t->disp("You open the " + o->get_name() + ".");
			o->open = true;
			o->describe(t, true, false);
		}
		else
		{
			t->disp("The " + o->get_name() + " is already open.");
		}
	}
	else if(o)
	{
		t->disp("You can't open a " + o->get_name() + "!");
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