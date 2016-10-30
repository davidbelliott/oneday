#pragma once

class EventSink;

#include "Event.h"
#include <map>
#include <queue>

class EventSource
{
protected:
	std::map<Event::EventType, std::vector<EventSink*>> listeners;
    std::queue<Event*> incoming_queue;

public:
	EventSource();
	virtual ~EventSource();

    /* Pushes an event on the incoming queue. */
    virtual void push_event(Event* event);

    /* Pops events off of the queue one by one, calls handle_event on them, and calls send_event on them. */
    virtual void handle_events();

	/*Registers the specified sink to receive events of the specified type.*/
	virtual void register_sink(EventSink* sink_to_register, Event::EventType type_to_register);

	/*Unregisters the specified sink from receiving events of the specified type.*/
	virtual void unregister_sink(EventSink* sink_to_unregister, Event::EventType type_to_unregister);

protected:
    /* Acts in response to an event. Called before this event is sent to children. */
    virtual void handle_event(Event* event);

	/* Sends an event to all relevant child listeners. */
	virtual void send_event(Event* event);

};
