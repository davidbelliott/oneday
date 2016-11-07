#include "GameState.h"
#include "Event.h"


GameState::GameState(Engine* engine_in)
:   Receiver(),
    engine(engine_in),
    terminal(),
    paused(false),
    running(true)
{
    terminal.owner = this;
}


GameState::~GameState()
{
}

void GameState::add_event(std::shared_ptr<Event> event)
{
    if(paused)
    {
        if(event->type == Event::KEY_PRESSED)
            paused = false;
    }
    else
    {
        Receiver::add_event(event);
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
