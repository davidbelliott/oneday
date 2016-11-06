#pragma once

#include "Event.h"
#include <queue>
#include <memory>

class Receiver
{
protected:
    std::queue<std::shared_ptr<Event>> mailbox;

public:
	Receiver();
	virtual ~Receiver();

    virtual void add_event(std::shared_ptr<Event> event);
};
