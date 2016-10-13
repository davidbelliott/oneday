#include "EventSink.h"

EventSink::EventSink()
{
}


EventSink::~EventSink()
{
}

void EventSink::catch_event(Event* event)
{
	events.push_back(event);
}

bool EventSink::get_event(Event*& event)
{
	if (events.size() > 0)
	{
		event = events.back();
		events.pop_back();
		return true;
	}
	else
	{
		event = 0;
		return false;
	}
}
