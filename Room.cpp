#include "Room.h"
#include "Terminal.h"

Room::Room(std::string name_in, std::string pretty_name_in, std::string description_in)
	: Object(name_in, description_in)
{
    pretty_name = pretty_name_in;
	for (size_t i = 0; i < DIRECTIONS_MAX; i++)
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

void Room::describe(Terminal* t, bool deep, bool describe_this)
{
    t->disp("You in " + pretty_name + ", boi.");
    Object::describe(t, deep, describe_this);
    for(int i = 0; i < DIRECTIONS_MAX; i++)
    {
        std::string dir_name = "";
        if((Direction)i == NORTH)
            dir_name = "To the north";
        else if((Direction)i == EAST)
            dir_name = "To the east";
        else if((Direction)i == SOUTH)
            dir_name = "To the south";
        else if((Direction)i == WEST)
            dir_name = "To the west";
        else if((Direction)i == UP)
            dir_name = "Up";
        else if((Direction)i == DOWN)
            dir_name = "Down";


        if(directions[i] != "")
        {
            Object* dir_room = parent->get_direct_child(directions[i], false);
            if(dir_room && dir_room->pretty_name != "")
                t->disp(dir_name + " is " + dir_room->pretty_name + ".");
        }
    }
}

