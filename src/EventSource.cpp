#include "EventSource.h"
#include "EventSink.h"
#include <algorithm>

EventSource::EventSource()
:   listeners(),
    incoming_queue()
{
}


EventSource::~EventSource()
{
}

void EventSource::push_event(Event* event)
{
    incoming_queue.push(event);
}

void EventSource::handle_event(Event* event)
{

}

void EventSource::send_event(Event* event)
{
	if (listeners.count(event->type) > 0)
	{
		for (size_t i = 0; i < listeners[event->type].size(); i++)
		{
			listeners[event->type][i]->notify(event);
		}
	}
}

void EventSource::handle_events()
{
    while(!incoming_queue.empty())
    {
        Event* front_event = incoming_queue.front();
        incoming_queue.pop();
        handle_event(front_event);
        send_event(front_event);
    }
}

void EventSource::register_sink(EventSink* sink_to_register, Event::EventType type_to_register)
{
	listeners[type_to_register].push_back(sink_to_register);
}

void EventSource::unregister_sink(EventSink* sink_to_unregister, Event::EventType type_to_unregister)
{
	listeners[type_to_unregister].erase(std::remove(listeners[type_to_unregister].begin(), listeners[type_to_unregister].end(), sink_to_unregister));
}
