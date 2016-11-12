#include "Terminal.h"
#include "Room.h"
#include "World.h"
#include "Event.h"
#include <iostream>


World::World()
	: Object(), active(true), player(NULL)
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

void World::set_current_room(std::string room_name)
{
	cur_room = room_name;
	Room* room = get_current_room();
    //clear screen
	if (!room)
	{
        std::cerr << "Error: room " + cur_room + " does not exist." << std::endl;
		exit(1);
	}
}
