#include "Receiver.h"

Receiver::Receiver()
{
}

Receiver::~Receiver()
{
}

void Receiver::add_event(std::shared_ptr<Event> event)
{
    mailbox.push(event);
}
