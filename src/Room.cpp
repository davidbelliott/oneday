#include "Room.h"
#include "Terminal.h"
#include "Config.h"
#include "Engine.h"
#include "Event.h"
#include "GameState.h"

Room::Room(std::string name_in, std::string pretty_name_in, std::string description_in)
	: Object(name_in, description_in)
{
    properties |= ROOM;
    pretty_name = pretty_name_in;
    for(int i = 0; i < DIRECTION_MAX; i++)
        directions[i] = "";
}

Room::Room()
	: Room("", "", "")
{
}

Room::~Room()
{
	//for (size_t i = 0; i < objects.size(); i++)
		//delete objects[i];
}
