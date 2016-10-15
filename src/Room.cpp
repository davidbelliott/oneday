#include "Room.h"
#include "Terminal.h"

Room::Room(std::string name_in, std::string pretty_name_in, std::string description_in)
	: Object(name_in, description_in)
{
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

void Room::describe(Terminal* t, bool deep, bool describe_this)
{
    t->set_color(sf::Color::Yellow);
    t->disp("You in " + pretty_name + ".");
    t->set_color();
    Object::describe(t, deep, describe_this);
    for(int i = 0; i < DIRECTION_MAX; i++)
    {
        if(directions[i] != "")
        {
            DirectionId dir_id = (DirectionId)i;
            Object* dir_room = parent->get_direct_child(directions[i], 0);
            if(dir_room && dir_room->pretty_name != "")
            {
                std::string dir_reference = dir[dir_id].dir_reference;
                t->disp(dir_reference + " is " + dir_room->pretty_name + ".");
            }
        }
    }
}

