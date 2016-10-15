#include "GameState.h"



GameState::GameState(Engine* engine_in)
: engine(engine_in), paused(false), running(true), blocks_update(false), blocks_draw(false)
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

void GameState::run(sf::Time dt)
{
	
}

void GameState::draw()
{
	
}
