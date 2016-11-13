#include "GameState.h"
#include "Terminal.h"

GameState::GameState(Engine* engine_in)
:   Observer(),
    terminal(new Terminal(this)),
    commands(),
    commands_priority(),
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
    else
    {
        terminal->notify(event);
    }
}

void GameState::send(cmd_ptr cmd)
{
    commands.push(cmd);
}

void GameState::send_front(cmd_ptr cmd)
{
    commands_priority.push(cmd);
}

void GameState::execute(cmd_ptr cmd)
{
    cmd->run_with_callbacks(this);
}

void GameState::execute_commands()
{
    while(!paused && (!commands_priority.empty() || !commands.empty()))
    {
        cmd_ptr cmd = nullptr;
        if(!commands_priority.empty())
        {
            cmd = commands_priority.front();
            commands_priority.pop();
        }
        else if(!commands.empty())
        {
            cmd = commands.front();
            commands.pop();
        }
        execute(cmd);
    }
}

void GameState::update(sf::Time dt)
{

}

void GameState::draw(sf::RenderTarget* target)
{
    terminal->draw(target);
}

void GameState::pause()
{
    paused = true;
}

void GameState::unpause()
{
    paused = false;
}
