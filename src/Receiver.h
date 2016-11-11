#pragma once

#include "Command.h"
#include "Event.h"
#include <queue>

class Receiver
{
protected:
    std::queue<cmd_ptr> mailbox;
    bool paused;

public:
	Receiver();
	virtual ~Receiver();

    virtual void send(cmd_ptr command);
    virtual void notify(event_ptr event);

    virtual void execute(cmd_ptr command);
    virtual void execute_commands();

    virtual void pause();
    virtual void unpause();
};
