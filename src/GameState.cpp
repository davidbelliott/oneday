#include "GameState.h"



GameState::GameState(Engine* engine_in)
: engine(engine_in), paused(false), blocks_update(false), blocks_draw(false)
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

void GameState::update(sf::Time dt)
{
	
}

void GameState::draw(sf::RenderTarget* target)
{
	
}
