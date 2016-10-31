#include "Action.h"
#include "Terminal.h"
#include "Room.h"
#include "World.h"
#include "Receiver.h"
#include "Event.h"
#include <iostream>


World::World()
	: Object(), active(true), change_room_action(NULL), player(NULL)
{
    if(!good_day.openFromFile("good_day.ogg"))
        std::cerr << "Error: unable to load good_day.ogg" << std::endl;
}

World::~World()
{
}

Room* World::get_current_room()
{
	if (has_direct_child(cur_room))
		return (Room*)get_direct_child(cur_room, 0);
	else
		return NULL;
}

void World::set_current_room(std::string room_name, Receiver* r)
{
	cur_room = room_name;
	Room* room = get_current_room();
	//e->terminal->clr();
	if (!room)
	{
	    r->add_event(new CmdDisp("Error: room " + cur_room + " does not exist."));
		exit(1);
	}
	else if(change_room_action)
	{
		change_room_action->run(this, r);
	}
}
