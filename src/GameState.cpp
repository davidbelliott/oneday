#include "GameState.h"
#include "World.h"

GameState::GameState(Engine* engine_in)
:   Observer(),
    engine(engine_in),
    running(true),
    world(nullptr)
{
}


GameState::~GameState()
{
}

void GameState::init()
{
    running = true;
}

void GameState::cleanup()
{

}

void GameState::notify(event_ptr event)
{

}

void GameState::update(unsigned int millis)
{

}

void GameState::draw()
{

}
