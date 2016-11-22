#pragma once

class Player;
#include "Object.h"
#include <SFML/Audio.hpp>
#include <map>

class World : public Object
{
public:
	std::map<std::string, int> flags;

    Player* player;


	std::string cur_room;
	bool active;
    sf::Music good_day;

	World();
	virtual ~World();

	Object* get_current_room();
	void set_current_room(std::string room_name);
};

