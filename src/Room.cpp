#include "Room.h"
#include "Terminal.h"
#include "Config.h"
#include "Engine.h"
#include "Event.h"
#include "GameState.h"

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

void Room::describe(GameState* g, bool deep, bool describe_this)
{
    g->send(std::make_shared<CmdSetColor>(config::colors[config::color_room_title]));
    g->send(std::make_shared<CmdDisp>("You in " + pretty_name + "."));
    g->send(std::make_shared<CmdSetColor>());
    Object::describe(g, deep, describe_this);
    for(int i = 0; i < DIRECTION_MAX; i++)
    {
        if(directions[i] != "")
        {
            DirectionId dir_id = (DirectionId)i;
            Object* dir_room = parent->get_direct_child(directions[i], 0);
            if(dir_room && dir_room->pretty_name != "")
            {
                std::string dir_reference = dir[dir_id].dir_reference;
                g->send(std::make_shared<CmdDisp>(dir_reference + " is " + dir_room->pretty_name + "."));
            }
        }
    }
}

