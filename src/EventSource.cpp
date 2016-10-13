#include "EventSink.h"
#include "EventSource.h"
#include <algorithm>

EventSource::EventSource()
{
}


EventSource::~EventSource()
{
}

void EventSource::send_event(Event* event)
{
	if (sinks.count(event->type) > 0)
	{
		for (size_t i = 0; i < sinks[event->type].size(); i++)
		{
			sinks[event->type][i]->catch_event(event);
		}
	}
	//delete event;
}

void EventSource::register_sink(EventSink* sink_to_register, Event::EventType type_to_register)
{
	sinks[type_to_register].push_back(sink_to_register);
}

void EventSource::unregister_sink(EventSink* sink_to_unregister, Event::EventType type_to_unregister)
{
	sinks[type_to_unregister].erase(std::remove(sinks[type_to_unregister].begin(), sinks[type_to_unregister].end(), sink_to_unregister));
}