#pragma once

class Event;

class EventSink
{
public:
	EventSink();
	virtual ~EventSink();

    /* Called by an EventSource to notify of an event */
	virtual void notify(Event* event);
};
