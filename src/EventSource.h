#pragma once

class EventSink;

#include "Event.h"
#include <map>
#include <vector>

class EventSource
{
private:
	std::map<Event::EventType, std::vector<EventSink*>> sinks;
public:
	EventSource();
	virtual ~EventSource();

	/*Sends an event to all relevant sinks. Deletes the event after notification.*/
	void send_event(Event* event);

	/*Registers the specified sink to receive events of the specified type.*/
	void register_sink(EventSink* sink_to_register, Event::EventType type_to_register);

	/*Unregisters the specified sink from receiving events of the specified type.*/
	void unregister_sink(EventSink* sink_to_unregister, Event::EventType type_to_unregister);
};

