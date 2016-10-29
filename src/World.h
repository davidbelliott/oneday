#pragma once

class Room;
class Player;
class Engine;
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
	Action* change_room_action;
    sf::Music good_day;

	World();
	virtual ~World();

	Room* get_current_room();
	void set_current_room(std::string room_name, Engine* e);
};

