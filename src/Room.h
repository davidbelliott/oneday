#pragma once

class GameState;

#include "Directions.h"
#include "Object.h"
#include "common.h"

class Room : public Object
{
public:

	Room(std::string name_in, std::string pretty_name_in, std::string description_in);
	Room();
	virtual ~Room();
};
