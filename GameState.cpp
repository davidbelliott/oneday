#include "GameState.h"



GameState::GameState(GameEngine* engine_in)
: engine(engine_in), paused(false), blocks_update(false), blocks_draw(false)
{
}


GameState::~GameState()
{
}

void GameState::set_engine(GameEngine* engine_in)
{
	engine = engine_in;
}

GameEngine* GameState::get_engine()
{
	return engine;
}

void GameState::set_paused(bool paused_in)
{
	paused = paused_in;
}

bool GameState::get_paused()
{
	return paused;
}

void GameState::set_blocks_update(bool blocks_update_in)
{
	blocks_update = blocks_update_in;
}

bool GameState::get_blocks_update()
{
	return blocks_update;
}

void GameState::set_blocks_draw(bool blocks_draw_in)
{
	blocks_draw = blocks_draw_in;
}

bool GameState::get_blocks_draw()
{
	return blocks_draw;
}

void GameState::init()
{

}

void GameState::cleanup()
{

}

void GameState::handle_events()
{
}

void GameState::update(sf::Time dt)
{
	
}

void GameState::draw(sf::RenderTarget* target)
{
	
}