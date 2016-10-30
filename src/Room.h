#pragma once

#include "Directions.h"
#include "Object.h"
#include "common.h"

class Room : public Object
{
public:

    std::string directions[DIRECTION_MAX];

	Room(std::string name_in, std::string pretty_name_in, std::string description_in);
	Room();
	virtual ~Room();

    virtual void describe(Engine* e, bool deep, bool describe_this);
};
