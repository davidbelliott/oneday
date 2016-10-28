#include "EventSink.h"

EventSink::EventSink()
    : event_queue()
{
}


EventSink::~EventSink()
{
}

void EventSink::notify(Event* event)
{
    event_queue.push(event);
}

Event* EventSink::pop_event()
{
    Event* top_event = nullptr;
    if(!event_queue.empty())
    {
        top_event = event_queue.front();
        event_queue.pop();
    }
    return top_event;
}
