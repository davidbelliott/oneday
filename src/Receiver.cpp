#include "Receiver.h"

Receiver::Receiver()
{
}

Receiver::~Receiver()
{
}

void Receiver::send(cmd_ptr cmd)
{
    mailbox.push(cmd);
}

void Receiver::notify(event_ptr event)
{

}

void Receiver::execute(cmd_ptr cmd)
{

}

void Receiver::execute(Command* cmd)
{
    cmd->run(this);
}

void Receiver::execute_commands()
{
    while(!mailbox.empty() && !paused)
    {
        cmd_ptr cmd = mailbox.front();
        mailbox.pop();
        execute(cmd);
    }
}

void Receiver::pause()
{
    paused = true;
}

void Receiver::unpause()
{
    paused = false;
}
