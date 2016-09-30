#pragma once

#include "Object.h"
#include "common.h"

class Room : public Object
{
public:

	Room(std::string name_in, std::string description_in);
	Room();
	virtual ~Room();
	std::string directions[DIRECTIONS_MAX];

    virtual void describe(Terminal* t, bool deep, bool describe_this);
};
