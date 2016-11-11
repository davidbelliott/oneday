#pragma once

#include "Event.h"

class Listener
{
public:
	Listener();
	virtual ~Listener();

    virtual void notify(event_ptr event);
};
