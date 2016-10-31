#pragma once

class Event;

#include <queue>

class Receiver
{
protected:
    std::queue<Event*> mailbox;

public:
	Receiver();
	virtual ~Receiver();

    virtual void add_event(Event* event);
};
