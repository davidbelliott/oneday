#include "GameState.h"



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

void GameState::init()
{
}

void GameState::cleanup()
{
}

void GameState::handle_event(Event* event)
{
}

void GameState::handle_events()
{
    while(!mailbox.empty())
    {
        Event* event = mailbox.front();
        mailbox.pop();
        handle_event(event);
    }
}

void GameState::run(sf::Time dt)
{
}

void GameState::draw(sf::RenderTarget* target)
{
}
