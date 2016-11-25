#include "Terminal.h"
#include "World.h"
#include "Event.h"
#include <iostream>


World::World()
	: Object(), active(true), player(nullptr)
{
    if(!good_day.openFromFile("good_day.ogg"))
        std::cerr << "Error: unable to load good_day.ogg" << std::endl;
}

World::~World()
{
}

Object* World::get_player()
{
    return player;
}

void World::set_player(Object* player_in)
{
    player = player_in;
}

Object* World::get_current_room()
{
	if(has_direct_child(cur_room))
		return get_direct_child(cur_room, 0);
	else
		return nullptr;
}

void World::set_current_room(std::string room_name)
{
	cur_room = room_name;
	Object* room = get_current_room();
    //clear screen
	if (!room)
	{
        std::cerr << "Error: room " + cur_room + " does not exist." << std::endl;
		exit(1);
	}
}
