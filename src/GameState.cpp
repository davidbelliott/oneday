#include "GameState.h"
#include "Terminal.h"

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
            unpause();
    }
}

void GameState::send(cmd_ptr cmd)
{
    commands.push(cmd);
}

void GameState::send_front(cmd_ptr cmd)
{
    //mailbox.push_front(cmd); FIXME!
}

void GameState::execute(cmd_ptr cmd)
{
    cmd->run(this);
}

void GameState::execute_commands()
{
    while(!commands.empty() && !paused)
    {
        cmd_ptr cmd = commands.front();
        commands.pop();
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
