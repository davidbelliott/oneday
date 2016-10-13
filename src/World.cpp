#include "Action.h"
#include "Terminal.h"
#include "Room.h"
#include "World.h"
#include <iostream>


World::World()
	: Object(), active(true), change_room_action(NULL), clothing(NULL)
{
}

World::~World()
{
}

Room* World::get_current_room()
{
	if (has_direct_child(cur_room))
		return (Room*)get_direct_child(cur_room, false);
	else
		return NULL;
}

void World::set_current_room(std::string room_name, Terminal* t)
{
	cur_room = room_name;
	Room* room = get_current_room();
	t->clr();
	if (!room)
	{
		t->disp("Error: room " + cur_room + " does not exist.");
		exit(1);
	}
	else if(change_room_action)
	{
		change_room_action->run(this, t);
	}
}