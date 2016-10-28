#pragma once

class Event;
#include <queue>

class EventSink
{
private:
    std::queue<Event*> event_queue;
public:
	EventSink();
	virtual ~EventSink();

    /* Called by an EventSource to notify of an event */
	virtual void notify(Event* event);

    /* Pops and returns the event at the front of the queue */
    virtual Event* pop_event();
};

