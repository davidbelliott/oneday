#include "GameState.h"



GameState::GameState(Engine* engine_in)
:   EventSink(),
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

void GameState::notify(Event* event)
{
}

void GameState::run(sf::Time dt)
{
}

void GameState::draw()
{
}
