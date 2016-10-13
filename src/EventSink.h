#pragma once

class Event;
#include <vector>

class EventSink
{
public:
	EventSink();
	virtual ~EventSink();

	virtual void handle_event(Event* event);
};

