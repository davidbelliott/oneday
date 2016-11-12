#include "GameState.h"

GameState::GameState(Engine* engine_in)
:   Observer(),
    terminal(new Terminal()),
    commands(),
    engine(engine_in),
    paused(false),
    running(true)
{
}


GameState::~GameState()
{
    delete terminal;
}

void GameState::notify(event_ptr event)
{
    if(paused)
    {
        if(event->type == Event::KEY_PRESSED)
            paused = false;
    }
}

void GameState::notify(event_ptr event)
{
    terminal.handle_event(event);
}

void GameState::send(cmd_ptr cmd)
{
    mailbox.push(cmd);
}

void GameState::execute(Command* cmd)
{
    cmd->run(this);
}

void GameState::execute_commands()
{
    while(!mailbox.empty() && !paused)
    {
        cmd_ptr cmd = mailbox.front();
        mailbox.pop();
        execute(cmd);
    }
}

void GameState::pause()
{
    paused = true;
}

void GameState::unpause()
{
    paused = false;
}
