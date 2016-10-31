#include "Receiver.h"

Receiver::Receiver()
{
}

Receiver::~Receiver()
{
}

void Receiver::add_event(Event* event)
{
    mailbox.push(event);
}
