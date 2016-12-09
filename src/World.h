#pragma once

class Player;
#include "Object.h"
#include <SFML/Audio.hpp>
#include <map>

class World : public Object
{
public:
	std::map<std::string, int> flags;

	std::string cur_room;
    Object* player;
	bool active;

	World();
	virtual ~World();

    Object* get_player();
    void set_player(Object* player_in);
	Object* get_current_room();
	void set_current_room(std::string room_name);
};

