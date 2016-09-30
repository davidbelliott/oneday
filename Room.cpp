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

void Room::describe(Terminal* t, bool deep, bool describe_this)
{
    Object::describe(t, deep, describe_this);
    for(int i = 0; i < DIRECTIONS_MAX; i++)
    {
        std::string dir_name = "";
        if((Direction)i == NORTH)
            dir_name = "north";
        else if((Direction)i == EAST)
            dir_name = "east";
        else if((Direction)i == SOUTH)
            dir_name = "south";
        else if((Direction)i == WEST)
            dir_name = "west";
        else if((Direction)i == UP)
            dir_name = "up";
        else if((Direction)i == DOWN)
            dir_name = "down";


        if(directions[i] != "")
            t->disp("To the " + dir_name + " is " + directions[i] + ".");
    }
}

