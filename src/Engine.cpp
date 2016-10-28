#include "EventSource.h"
#include "Engine.h"
#include "GameState.h"
#include "Room.h"
#include "World.h"
#include "Terminal.h"
#include "common.h"

Engine::Engine(Terminal* terminal_in)
: EventSource(),
    paused(false),
    running(true),
    terminal(terminal_in)
{
}


Engine::~Engine()
{
}

void Engine::push_state(GameState* state)
{
    game_states.push_back(state);
    state->init();
}

void Engine::pop_state()
{
    if(!game_states.empty())
    {
        game_states.back()->cleanup();
        game_states.pop_back();
    }
}

void Engine::handle_events()
{
    if(!game_states.empty())
        game_states.back()->handle_events();
}

void Engine::draw()
{
    if(!game_states.empty())
        game_states.back()->draw();
    terminal->draw();
}

void Engine::run(sf::Time dt)
{
    if(!game_states.empty())
        game_states.back()->run(dt);
    while(game_states.size() > 0 && !game_states.back()->running)
        pop_state();
    if(game_states.size() == 0)
        running = false;
}
