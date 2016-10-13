#pragma once

#include "Action.h"
#include "Word.h"

#include <map>

class ActionFactory
{
public:
	ActionFactory();
	virtual ~ActionFactory();

	Action* create_action(Word word);
};

