#include "Room.h"
#include "Terminal.h"

Room::Room(std::string name_in, std::string description_in)
	: Object(name_in, description_in)
{
	for (size_t i = 0; i < DIRECTIONS_MAX; i++)
		directions[i] = "";
}

Room::Room()
	: Room("", "")
{
}

Room::~Room()
{
	//for (size_t i = 0; i < objects.size(); i++)
		//delete objects[i];
}