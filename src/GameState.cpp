#include "GameState.h"
#include "Event.h"


GameState::GameState(Engine* engine_in)
:   Receiver(),
    engine(engine_in),
    paused(false),
    running(true)
{
}


GameState::~GameState()
{
}

void GameState::notify(event_ptr event)
{
    if(paused)
    {
        if(event->type == Event::KEY_PRESSED)
            paused = false;
    }
}

void GameState::init()
{
}

void GameState::cleanup()
{
}

void GameState::handle_event(std::shared_ptr<Event> event)
{
    terminal.handle_event(event);
}

void GameState::handle_events()
{
    while(!mailbox.empty() && !paused)
    {
        std::shared_ptr<Event> event = mailbox.front();
        mailbox.pop();
        handle_event(event);
    }
}

void GameState::run(sf::Time dt)
{
}

void GameState::draw(sf::RenderTarget* target)
{
    terminal.draw(target);
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
