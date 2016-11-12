#pragma once

#include "Event.h"

class Observer
{
public:
	Observer();
	virtual ~Observer();

    virtual void notify(event_ptr event);
};
