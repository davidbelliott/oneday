#include "GameState.h"



GameState::GameState(Engine* engine_in)
: engine(engine_in), paused(false), running(true)
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

void GameState::handle_events()
{
    Event* top_event = nullptr;
    while(top_event = pop_event())
        handle_event(top_event);
}

void GameState::handle_event(Event* event)
{
}

void GameState::run(sf::Time dt)
{
}

void GameState::draw()
{
}
