#include "Terminal.h"
#include "World.h"
#include "Event.h"
#include <iostream>


World::World()
	: Object(), active(true), player(nullptr)
{
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
    return player->parent;
}

void World::set_current_room(std::string room_name)
{
    Object* room = get_indirect_child(room_name, 0);
    if(room)
    {
        cur_room = room_name;
        if(player->parent)
            player->parent->remove_child(player);
        room->add_child(player);
    }
    else
    {
        std::cerr << "Error: room " + room_name + " does not exist." << std::endl;
    }
}
