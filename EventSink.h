#pragma once

class Event;
#include <vector>

class EventSink
{
private:
	std::vector<Event*> events;
public:
	EventSink();
	virtual ~EventSink();

	virtual void catch_event(Event* event);
	virtual bool get_event(Event*& event);
};

